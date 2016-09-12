#ifndef FV_PROGRAM_H
#define FV_PROGRAM_H

#include "Shader.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "Texture.h"
#include <memory>

namespace FV {

/**
 * @brief Encapsulates an OpenGL program.
 *
 * This class lets you load and compile OpenGL shader programs quickly. It is
 * your responsibility to bind and unbind the program by calling Use() and
 * Release() respectively. Trying to set uniforms and attributes on an unbound
 * program is bound to result in trouble.
 */
class Program
{
public:
    /**
     * @brief Creates a new OpenGL progarm.
     * @param vertexSource The source code of the vertex shader, exactly the way
     *                     it should be compiled.
     * @param fragmentSource The source code of the fragment shader, exactly the
     *                       way it should be compiled.
     */
    Program(std::string vertexSource, std::string fragmentSource);
    ~Program();

    /**
     * @brief Makes the program the currently bound OpenGL program.
     *
     * @note You don't need to release a previous program to bind another one.
     */
    void Use();

    /**
     * @brief Unbinds this program.
     */
    void Release();

    /**
     * @brief Represents an OpenGL uniform.
     */
    class Uniform
    {
    public:
        Uniform(GLint id);
        Uniform();

        inline GLint GetID() const {
            return m_ID;
        }

        void Set(float v) const;
        void Set(double v) const;
        void Set(int v) const;
        void Set(glm::vec2 v) const;
        void Set(glm::dvec2 v) const;
        void Set(glm::ivec2 v) const;
        void Set(glm::vec3 v) const;
        void Set(glm::dvec3 v) const;
        void Set(glm::ivec3 v) const;
        void Set(glm::vec4 v) const;
        void Set(glm::dvec4 v) const;
        void Set(glm::ivec4 v) const;
        void Set(glm::mat2 m) const;
        void Set(glm::dmat2 m) const;
        void Set(glm::mat3 m) const;
        void Set(glm::dmat3 m) const;
        void Set(glm::mat4 m) const;
        void Set(glm::dmat4 m) const;
        void Set(std::shared_ptr<Texture> texture, int unit) const;

    private:
        GLint m_ID;
    };

    /**
     * @brief Returns a Uniform object corresponding to an OpenGL uniform.
     * @param name The name of the uniform.
     * @return A uniform object.
     * @note The program does not need to be bound.
     * An assertion failure will be thrown if the uniform is not found.
     */
    Uniform GetUniform(std::string name) const;

public:
    static std::string s_shaderPath;

protected:

    //! The OpenGL object ID of the program.
    GLint m_ID;
};

}
#endif // FV_PROGRAM_H
