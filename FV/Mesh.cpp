#include "Mesh.h"
#include "assimp/scene.h"
using namespace FV;
using namespace std;

Mesh::Mesh(const aiMesh *mesh) :
    indexBuffer(IndexBuffer::INDEX)
{
    // We only support a single UV vector per vertex.
    assert(mesh->GetNumUVChannels() == 1);

    vector<Pos3N3UV2> vertices;
    vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Pos3N3UV2 vertex = {
            {
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            },
            {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            },
            (mesh->mTextureCoords[0] == nullptr) ? glm::vec2(0.0f, 0.0f) :
                glm::vec2(mesh->mTextureCoords[0][i].x,
                          mesh->mTextureCoords[0][i].y)
        };
        vertices.push_back(vertex);
    }

    vector<unsigned int> indices;
    indices.reserve(mesh->mNumFaces * 3); // aiProcess_Triangulate
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    VAO.Bind();
    vertexBuffer.Bind();
    vertexBuffer.SetData(vertices.data(), vertices.size());
    vertexBuffer.SetAttribPointer(0, 3, offsetof(Pos3N3UV2, position));
    vertexBuffer.SetAttribPointer(1, 3, offsetof(Pos3N3UV2, normal));
    vertexBuffer.SetAttribPointer(2, 2, offsetof(Pos3N3UV2, UV));
    indexBuffer.Bind();
    indexBuffer.SetData(indices.data(), indices.size());
    indexCount = indices.size();
}
