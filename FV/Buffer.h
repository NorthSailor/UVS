#ifndef FV_BUFFER_H
#define FV_BUFFER_H

#include <GL/glew.h>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace FV {

/**
 * Represents an OpenGL buffer object.
 */
template <typename T>
class Buffer
{
public:
    //! Specifies the type of data a buffer will contain.
    enum Type {
        VERTEX = GL_ARRAY_BUFFER, //!< Vertex attributes.
        INDEX = GL_ELEMENT_ARRAY_BUFFER, //!< Vertex indices.
        /// @todo Add the rest of the OpenGL buffer types.
    };

    //! Specifies the update pattern for the buffer.
    enum UsagePattern {
        STATIC = GL_STATIC_DRAW, //!< The data is unlikely to change.
        DYNAMIC = GL_DYNAMIC_DRAW, //!< The data is likely to change a lot.
        STREAM = GL_STREAM_DRAW //!< The data is likely to change at each frame.
    };

    /**
     * Creates a new buffer of the specified type and usage.
     * @param type The type for the buffer. @see Type
     * @param usage The usage pattern for the buffer. @see UsagePattern
     */
    Buffer(Type type = VERTEX, UsagePattern usage = STATIC);

    /**
     * Destroyes the buffer and the underlying OpenGL object.
     */
    ~Buffer();

    /**
     * Binds the buffer. Necessary before calling most functions.
     */
    void Bind() const;

    /**
     * Sets the buffer's data to the provided memory segment.
     * @param pData The new data of the buffer.
     * @param size The size of the data in bytes.
     * @note The buffer must be bound with Bind().
     */
    void SetData(T *pData, int count);

    /**
     * Configures a new vertex attribute.
     * @param index The location of the attribute in the vertex shader.
     * @param size The number of components of the attribute.
     * @param offset The offset from the first attribute of the buffer.
     * @param stride The stride of all attributes.
     * @param type The data type of every component of the attribute.
     * @note The buffer must be bound with Bind().
     */
    void SetAttribPointer(GLuint index, int size = 3, size_t offset = 0,
                          size_t stride = sizeof(T),
                          GLenum type = GL_FLOAT);

protected:
    Type m_type;
    UsagePattern m_usage;
    GLuint m_ID;
};

typedef Buffer<glm::vec3> Buffer3f;
typedef Buffer<glm::vec2> Buffer2f;
typedef Buffer<float> BufferF;
typedef Buffer<double> BufferD;
typedef Buffer<unsigned int> BufferUI, IndexBuffer;
typedef Buffer<char> DataBuffer;

template <typename T>
Buffer<T>::Buffer(Type type, UsagePattern usage) :
    m_type(type),
    m_usage(usage)
{
    glGenBuffers(1, &m_ID);
    assert(m_ID > 0);
}

template <typename T>
Buffer<T>::~Buffer()
{
    glDeleteBuffers(1, &m_ID);
}

template <typename T>
void Buffer<T>::Bind() const
{
    glBindBuffer(static_cast<GLenum>(m_type), m_ID);
}

template <typename T>
void Buffer<T>::SetData(T *pData, int count)
{
    /// @todo For OpenGL 4.5+ we should use glNamedBufferData() instead.
    glBufferData(static_cast<GLenum>(m_type), count * sizeof(T),
                 static_cast<void*>(pData),
                 static_cast<GLenum>(m_usage));
}

template <typename T>
void Buffer<T>::SetAttribPointer(GLuint index, int size,
                                 size_t offset, size_t stride,
                                 GLenum type)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void*)offset);
}

}

#endif // FV_BUFFER_H
