#ifndef FV_MESH_H
#define FV_MESH_H

#include <vector>
#include "VertexArray.h"
#include "Buffer.h"
#include "Common.h"
#include "Program.h"
#include "Texture2D.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct aiMesh;

namespace FV {

typedef std::vector<std::shared_ptr<Texture2D>>::iterator TextureIterator;
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
class Mesh
{
public:

    Mesh(const aiMesh *mesh);

    struct Pos3N3UV2 {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 UV;
    };

    VertexArray VAO;
    Buffer<Pos3N3UV2> vertexBuffer;
    IndexBuffer indexBuffer;
    int indexCount;
    MaterialIterator material;
};

}

#endif // FV_MESH_H
