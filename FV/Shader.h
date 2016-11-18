#ifndef FV_SHADER_H
#define FV_SHADER_H

#include "OpenGL.h"
#include <string>

namespace FV {

/**
 * @brief Encapsulates an OpenGL shader object.
 */
class Shader
{
public:
    enum ShaderType {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        TESS_CONTROL = GL_TESS_CONTROL_SHADER,
        TESS_EVAL = GL_TESS_EVALUATION_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER
        // No macOS support for OpenGL compute shaders!
    };

    /**
     * @brief Creates a new shader.
     * @param type The type of the shader to create.
     * @param source The complete source code of the shader.
     */
    Shader(ShaderType type, std::string source);
    ~Shader();

    /** Gets the underlying OpenGL ID. */
    inline GLuint GetID() const {
        return m_ID;
    }

private:
    GLuint m_ID;
};

}

#endif // FV_SHADER_H
