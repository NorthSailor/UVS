#include "Model.h"
#include "ObjectLoader.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#include "glm/gtc/type_ptr.hpp"
using namespace FV;
using namespace std;
using namespace glm;

Model::Node GetNodeRecursive(aiNode *aNode, vector<shared_ptr<Mesh>>& meshes)
{
    Model::Node node;
    node.transform = glm::make_mat4((const float*)(&aNode->mTransformation));

    node.name = aNode->mName.C_Str();

    for (unsigned int i = 0; i < aNode->mNumMeshes; i++)
        node.meshes.push_back(meshes.begin() + aNode->mMeshes[i]);

    for (unsigned int i = 0; i < aNode->mNumChildren; i++)
        node.children.push_back(GetNodeRecursive(aNode->mChildren[i], meshes));

    return node;
}

inline int GetTotalTextureCount(const aiScene *scene)
{
    int totalCount = 0;
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial *amaterial = scene->mMaterials[i];
        totalCount += amaterial->GetTextureCount(aiTextureType_DIFFUSE);
        totalCount += amaterial->GetTextureCount(aiTextureType_NORMALS);
        totalCount += amaterial->GetTextureCount(aiTextureType_SPECULAR);
    }
    return totalCount;
}

Model::Model(string filename, ObjectLoader &loader)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename,
                                             aiProcess_Triangulate |
                                             aiProcess_FlipUVs);
    if (scene == nullptr)
        throw Exception("Failed to load model: " + filename);

    /// @todo UGLY HACK - Remove the "Resources/" prefix.
    string path;
    {
        auto first = filename.find_first_of('/') + 1;
        auto last = filename.find_last_of('/') + 1;
        path = filename.substr(first, last - first);
    }

    // We need to find the total number of textures beforehand so that
    // the iterators won't be invalidated.
    m_textures.reserve(GetTotalTextureCount(scene));

    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial *amaterial = scene->mMaterials[i];
        assert(amaterial != nullptr);
        Material material;
        material.diffuseTexture = m_textures.end();
        material.normalsTexture = m_textures.end();
        material.specularTexture = m_textures.end();

        int count = amaterial->GetTextureCount(aiTextureType_DIFFUSE);
        for (int i = 0; i < count; i++) {
            aiString str;
            amaterial->GetTexture(aiTextureType_DIFFUSE, i, &str);
            material.diffuseTexture = m_textures.insert(m_textures.end(),
                        loader.LoadTexture2D(path + str.C_Str()));
        }

        count = amaterial->GetTextureCount(aiTextureType_NORMALS);
        for (int i = 0; i < count; i++) {
            aiString str;
            amaterial->GetTexture(aiTextureType_NORMALS, i, &str);
            material.normalsTexture = m_textures.insert(m_textures.end(),
                        loader.LoadTexture2D(path + str.C_Str()));
        }

        count = amaterial->GetTextureCount(aiTextureType_SPECULAR);
        for (int i = 0; i < count; i++) {
            aiString str;
            amaterial->GetTexture(aiTextureType_SPECULAR, i, &str);
            material.specularTexture = m_textures.insert(m_textures.end(),
                        loader.LoadTexture2D(path + str.C_Str()));
        }

        m_materials.push_back(material);
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        shared_ptr<Mesh> mesh = make_shared<Mesh>(scene->mMeshes[i]);
        mesh->material = m_materials.begin() +
                scene->mMeshes[i]->mMaterialIndex;
        m_meshes.push_back(mesh);
    }

    m_rootNode = GetNodeRecursive(scene->mRootNode, m_meshes);

    importer.FreeScene();
}

void Model::Draw(shared_ptr<Program> program, const RenderingUniforms &uniforms)
{
    DrawNodeRecursive(mat4(), m_rootNode, program, uniforms);
}

void Model::DrawNodeRecursive(mat4 transform, const Node &node,
                              shared_ptr<Program> program,
                              const RenderingUniforms &uniforms)
{
    mat4 currentTransform = transform * node.transform;
    uniforms.transform.Set(currentTransform);

    for (auto meshIt : node.meshes) {
        shared_ptr<Mesh> mesh = *meshIt;
        mesh->VAO.Bind();
        Material mat = *(mesh->material);
        if (mat.diffuseTexture != m_textures.end())
            uniforms.diffuseSampler.Set(*(mat.diffuseTexture), 0);

        glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, (void*)0);
    }

    for (const Node& child : node.children) {
        DrawNodeRecursive(currentTransform, child, program, uniforms);
    }
}
