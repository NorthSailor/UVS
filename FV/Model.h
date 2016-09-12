#ifndef FV_MODEL_H
#define FV_MODEL_H

#include <vector>
#include <memory>
#include "glm/mat4x4.hpp"
#include "Mesh.h"
#include "Texture2D.h"
#include "Program.h"

namespace FV {

class ObjectLoader;

/**
 * A model loadable from a file.
 */
class Model
{
public:
    /**
     * Creates a new model.
     * @param filename Path to the model file.
     * @param loader The resource loader to use for loading the model and its
     * 	      textures.
     */
    Model(std::string filename, ObjectLoader &loader);

    typedef std::vector<std::shared_ptr<Mesh>>::iterator MeshIterator;
    struct Node
    {
        /** A name given by the model artist. */
        std::string name;
        /**
         * Children models.
         */
        std::vector<Node> children;
        /**
         * Meshes.
         */
        std::vector<MeshIterator> meshes;
        /**
         * Transform relative to its parent. World transform for the root node.
         * The product of the transforms in the model hierarchy gives the model
         * matrix.
         */
        glm::mat4 transform;
    };

    /**
     * Contains the required uniforms for rendering a model with an FV::Program.
     */
    struct RenderingUniforms
    {
        Program::Uniform transform; //! Contains the complete transform.
        Program::Uniform diffuseSampler; //! The diffuse texture.
    };

    /**
     * Renders a model on the current framebuffer.
     * @param program The program to use. Must support the required uniforms.
     * @param uniforms The uniforms valid for the given program.
     */
    void Draw(std::shared_ptr<Program> program,
              const RenderingUniforms &uniforms);

protected:
    Node m_rootNode;
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::vector<std::shared_ptr<Texture2D>> m_textures;
    std::vector<Material> m_materials;

    void DrawNodeRecursive(glm::mat4 transform, const Node& node,
                           std::shared_ptr<Program> program,
                           const RenderingUniforms &uniforms);
};

}

#endif // FV_MODEL_H
