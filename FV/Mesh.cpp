#include "Mesh.h"
#include <assimp/scene.h>
using namespace FV;
using namespace std;

shared_ptr<ModelMesh> FV::parse_mesh_from_assimp(aiMesh *aimesh)
{
    // We only support a single UV vector per vertex.
    assert(aimesh->GetNumUVChannels() == 1);

    vector<Pos3N3UV2> vertices;
    vertices.reserve(aimesh->mNumVertices);
    for (unsigned int i = 0; i < aimesh->mNumVertices; i++) {
        Pos3N3UV2 vertex = {
            {
                aimesh->mVertices[i].x,
                aimesh->mVertices[i].y,
                aimesh->mVertices[i].z
            },
            {
                aimesh->mNormals[i].x,
                aimesh->mNormals[i].y,
                aimesh->mNormals[i].z
            },
            (aimesh->mTextureCoords[0] == nullptr) ? glm::vec2(0.0f, 0.0f) :
                glm::vec2(aimesh->mTextureCoords[0][i].x,
                          aimesh->mTextureCoords[0][i].y)
        };
        vertices.push_back(vertex);
    }

    vector<unsigned int> indices;
    indices.reserve(aimesh->mNumFaces * 3); // aiProcess_Triangulate
    for (unsigned int i = 0; i < aimesh->mNumFaces; i++) {
        aiFace face = aimesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    shared_ptr<ModelMesh> mesh = make_shared<ModelMesh>();
    mesh->VAO.Bind();
    mesh->vertexBuffer.Bind();
    mesh->vertexBuffer.SetData(vertices.data(), (int)vertices.size());
    mesh->vertexBuffer.SetAttribPointer(0, 3, offsetof(Pos3N3UV2, position));
    mesh->vertexBuffer.SetAttribPointer(1, 3, offsetof(Pos3N3UV2, normal));
    mesh->vertexBuffer.SetAttribPointer(2, 2, offsetof(Pos3N3UV2, UV));
    mesh->indexBuffer.Bind();
    mesh->indexBuffer.SetData(indices.data(), (int)indices.size());
    mesh->indexCount = (int)indices.size();
    return mesh;
}

shared_ptr<Mesh<Pos2UV2>> FV::create_quad_mesh()
{
    Pos2UV2 vertices[] = {
        { { -1.0f, -1.0f }, { 0.0f, 0.0f } },
        { { 1.0f, -1.0f }, { 1.0f, 0.0f } },
        { { 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f, 1.0f }, { 0.0f, 1.0f } }
    };

    GLuint indices[] = { 0, 1, 2, 0, 2, 3 };

    shared_ptr<Mesh<Pos2UV2>> mesh = make_shared<Mesh<Pos2UV2>>();
    mesh->VAO.Bind();
    mesh->vertexBuffer.Bind();
    mesh->vertexBuffer.SetData(vertices, 4);
    mesh->vertexBuffer.SetAttribPointer(0, 2, offsetof(Pos2UV2, position));
    mesh->vertexBuffer.SetAttribPointer(1, 2, offsetof(Pos2UV2, UV));
    mesh->indexBuffer.Bind();
    mesh->indexBuffer.SetData(indices, 6);
    mesh->indexCount = 6;
    return mesh;
}
