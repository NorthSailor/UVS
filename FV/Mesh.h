#ifndef FV_MESH_H
#define FV_MESH_H

#include <vector>
#include "VertexArray.h"
#include "Buffer.h"
#include "Common.h"
#include "Program.h"
#include "Texture.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct aiMesh;

namespace FV {

struct Pos3N3UV2 {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 UV;
};

// Mainly for drawing quads.
struct Pos2UV2 {
    glm::vec2 position;
    glm::vec2 UV;
};

typedef std::vector<std::shared_ptr<Texture>>::iterator TextureIterator;
struct Material {
    TextureIterator diffuseTexture;
    TextureIterator specularTexture;
    TextureIterator normalsTexture;
};
typedef std::vector<Material>::iterator MaterialIterator;

/**
 * Represents a mesh. A mesh is the final element to be rendered consisting of
 * only the vertices, indices, textures and other rendering information.
 * It does not carry any semantic information like orientation, hierarchy or
 * transformations.
 */
template <typename V>
class Mesh
{
public:
    Mesh();

    VertexArray VAO;
    Buffer<V> vertexBuffer;
    IndexBuffer indexBuffer;
    int indexCount;
    MaterialIterator material;

};

template <typename V>
Mesh<V>::Mesh() :
    indexBuffer(FV::IndexBuffer::INDEX)
{
}

typedef Mesh<Pos3N3UV2> ModelMesh;

std::shared_ptr<ModelMesh> parse_mesh_from_assimp(aiMesh *aimesh);

std::shared_ptr<Mesh<Pos2UV2>> create_quad_mesh();

}

#endif // FV_MESH_H
