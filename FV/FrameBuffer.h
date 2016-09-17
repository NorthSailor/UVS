#ifndef FV_FRAMEBUFFER_H
#define FV_FRAMEBUFFER_H

#include <GL/glew.h>
#include "Mesh.h"

namespace FV {

/**
 * Represents an OpenGL framebuffer object. It can also encapsulate the default
 * framebuffer object but the RAII scheme does not apply for obvious reasons.
 */
class FrameBuffer
{
public:
    //! Framebuffer binding target.
    enum Target {
        READ = GL_READ_FRAMEBUFFER,
        DRAW = GL_DRAW_FRAMEBUFFER,
        READ_DRAW = GL_FRAMEBUFFER
    };

    enum BufferBit {
        COLOR_BIT = GL_COLOR_BUFFER_BIT,
        DEPTH_BIT = GL_DEPTH_BUFFER_BIT,
        STENCIL_BIT = GL_STENCIL_BUFFER_BIT,
        ACCUM_BIT = GL_ACCUM_BUFFER_BIT,
        COLOR_DEPTH_BIT = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
    };

    enum Attachment {
        COLOR0 = GL_COLOR_ATTACHMENT0,
        COLOR1 = GL_COLOR_ATTACHMENT1,
        COLOR2 = GL_COLOR_ATTACHMENT2,
        COLOR3 = GL_COLOR_ATTACHMENT3,
        COLOR4 = GL_COLOR_ATTACHMENT4,
        COLOR5 = GL_COLOR_ATTACHMENT5,
        COLOR6 = GL_COLOR_ATTACHMENT6,
        COLOR7 = GL_COLOR_ATTACHMENT7,
        COLOR8 = GL_COLOR_ATTACHMENT8,
        COLOR9 = GL_COLOR_ATTACHMENT9,
        COLOR10 = GL_COLOR_ATTACHMENT10,
        COLOR11 = GL_COLOR_ATTACHMENT11,
        COLOR12 = GL_COLOR_ATTACHMENT12,
        COLOR13 = GL_COLOR_ATTACHMENT13,
        COLOR14 = GL_COLOR_ATTACHMENT14,
        COLOR15 = GL_COLOR_ATTACHMENT15,
        DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT,
        DEPTH = GL_DEPTH_ATTACHMENT,
        STENCIL = GL_STENCIL_ATTACHMENT
    };

    //!
    //! Creates a new framebuffer object.
    //! \param onscreen Wether the object should represent the default
    //! 	   			framebuffer.
    //!
    FrameBuffer(bool onscreen = false);
    ~FrameBuffer();

    //! Binds the framebuffer to the specified target.
    void Bind(Target target = READ_DRAW);

    //! Restores the default framebuffer at the target where it is currently
    //! bound. Same as creating a class for the default framebuffer and binding
    //! it.
    static void RestoreDefault(Target target = READ_DRAW);

    //! Returns whether the framebuffer is complete (ready to be used).
    //! @note The framebuffer must be bound.
    bool IsComplete() const;

    //! Gets the OpenGL ID of the framebuffer.
    inline GLuint GetID() const {
        return m_ID;
    }

    /**
     * Draws a quad on the screen with the currently active program.
     * The vertex shader must accept 2D vertex location at attribute zero and
     * UV coordinates at attribute 1. The locations are in normal device
     * coordinates.
     */
    void DrawQuad();

    /**
     * Sets the clear color of the framebuffer.
     * @param color The new color.
     * @note This does not clear the framebuffer in any way!
     */
    void SetClearColor(glm::vec4 color);

    /**
     * Clears the framebuffer.
     * @param bit The bits to clear.
     */
    void Clear(BufferBit bit = COLOR_DEPTH_BIT);

    /**
     * An OpenGL renderbuffer to be used with a frame buffer.
     */
    class RenderBuffer {
    public:
        RenderBuffer();
        ~RenderBuffer();

        /** Binds the render buffer to the default target. */
        void Bind();

        /**
         * Configures the storage of the renderbuffer for 1 sample.
         * @param width The width of the buffer in pixels.
         * @param height The height of the buffer in pixels.
         * @param format The internal storage format to be used.
         * @note The render buffer must be bound with Bind().
         */
        void SetStorage(int width, int height, Texture::Format format);

        /**
         * Configures the storage of the render buffer for multiple samples.
         * @param width The width of the buffer in pixels.
         * @param height The height of the buffer in pixels.
         * @param samples The number of samples to use.
         * @param format The internal storage format to be used.
         * @note The render buffer must be bound with Bind().
         */
        void SetStorageMultisample(int width, int height, int samples,
                                   Texture::Format format);

        //! Gets the OpenGL ID of the renderbuffer.
        inline GLuint GetID() const {
            return m_ID;
        }

    protected:
        GLuint m_ID;
    };

    /**
     * Attaches a texture to the framebuffer.
     * @param a The attachment where the texture should be attached.
     * @param texture The texture to attach.
     */
    void Attach(Attachment a, std::shared_ptr<Texture> texture);

    /**
     * Attaches a renderbuffer to the framebuffer.
     * @param a The attachment where the renderbuffer should be attached.
     * @param rb The renderbuffer to attach.
     */
    void Attach(Attachment a, const RenderBuffer &rb);

    /**
     * Blits from the framebuffer bound as FrameBuffer::READ to the framebuffer
     * bound as FrameBuffer::DRAW.
     */
    static void Blit(int x1, int y1, int w1, int h1,
                     int x2, int y2, int w2, int h2,
                     BufferBit bit = COLOR_BIT,
                     Texture::FilteringMethod filtering = Texture::NEAREST);

private:
    GLuint m_ID;
    Target m_target;

    static std::shared_ptr<Mesh<Pos2UV2>> m_quad;
    static void CreateQuadMesh();
};

}

#endif // FV_FRAMEBUFFER_H
