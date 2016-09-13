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

    enum ClearBit {
        COLOR = GL_COLOR_BUFFER_BIT,
        DEPTH = GL_DEPTH_BUFFER_BIT,
        STENCIL = GL_STENCIL_BUFFER_BIT,
        ACCUM = GL_ACCUM_BUFFER_BIT,
        COLOR_DEPTH = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
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

    void DrawQuad();

    void SetClearColor(glm::vec4 color);
    void Clear(ClearBit bit = COLOR_DEPTH);

private:
    GLuint m_ID;
    Target m_target;

    static std::shared_ptr<Mesh<Pos2UV2>> m_quad;
    static void CreateQuadMesh();
};

}

#endif // FV_FRAMEBUFFER_H
