#ifndef FV_VERTEXARRAY_H
#define FV_VERTEXARRAY_H

#include <GL/glew.h>

namespace FV {

/**
 * Encapsulates an OpenGL vertex array object (or VAO).
 */
class VertexArray
{
public:
    /** Creates a new VAO. */
    VertexArray();

    /** Destroyes the VAO. */
    ~VertexArray();

    /** Binds the VAO. */
    void Bind();

    /** Gets the OpenGL ID of the VAO. */
    inline GLuint GetID() const {
        return m_ID;
    }

private:
    GLuint m_ID;
};

}

#endif // FV_VERTEXARRAY_H
