#include "FrameBuffer.h"
#include "Common.h"
using namespace FV;
using namespace std;

FrameBuffer::FrameBuffer(bool onscreen) :
    m_target(READ_DRAW)
{
    if (onscreen)
        m_ID = 0;
    else
        glGenFramebuffers(1, &m_ID);
}

FrameBuffer::~FrameBuffer()
{
    if (m_ID > 0)
        glDeleteFramebuffers(1, &m_ID);
}

void FrameBuffer::Bind(Target target)
{
    glBindFramebuffer(target, m_ID);
    m_target = target;
    assert(glGetError() == 0);
}

void FrameBuffer::RestoreDefault(Target target)
{
    glBindFramebuffer(target, 0);
}

bool FrameBuffer::IsComplete() const {
    return glCheckFramebufferStatus(m_target) == GL_FRAMEBUFFER_COMPLETE;
}

shared_ptr<Mesh<Pos2UV2>> FV::FrameBuffer::m_quad;

void FV::FrameBuffer::CreateQuadMesh()
{
    if (m_quad != nullptr)
        return; // The quad is ready.
    m_quad = create_quad_mesh();
}

void FrameBuffer::DrawQuad()
{
    CreateQuadMesh();
    m_quad->VAO.Bind();
    glDrawElements(GL_TRIANGLES, m_quad->indexCount, GL_UNSIGNED_INT, (void*)0);
}

void FrameBuffer::SetClearColor(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void FrameBuffer::Clear(BufferBit bit)
{
    glClear(bit);
}

FrameBuffer::RenderBuffer::RenderBuffer()
{
    glGenRenderbuffers(1, &m_ID);
}

FrameBuffer::RenderBuffer::~RenderBuffer()
{
    glDeleteRenderbuffers(1, &m_ID);
}

void FrameBuffer::RenderBuffer::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
}

void FrameBuffer::RenderBuffer::SetStorage(int width, int height,
                                           Texture::Format format)
{
    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
}

void FrameBuffer::RenderBuffer::SetStorageMultisample(int width, int height,
                                                      int samples,
                                                      Texture::Format format)
{
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples,
        format, width, height);
}

void FrameBuffer::Attach(Attachment a, shared_ptr<Texture> texture)
{
    switch (texture->GetTarget()) {
    case Texture::TEX_2D:
    case Texture::TEX_2D_MULTISAMPLE:
        glFramebufferTexture2D(GL_FRAMEBUFFER, a,
                               texture->GetTarget(), texture->GetID(), 0);
        break;
    default:
        throw Exception("Don't know how to attach other texture types.");
    }
}

void FrameBuffer::Attach(Attachment a, const RenderBuffer& rb)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, a, GL_RENDERBUFFER, rb.GetID());
}

void FrameBuffer::Blit(int x1, int y1, int w1, int h1,
                       int x2, int y2, int w2, int h2,
                       FrameBuffer::BufferBit bit,
                       Texture::FilteringMethod filtering)
{
    glBlitFramebuffer(x1, y1, w1, h1, x2, y2, w2, h2, bit, filtering);
}
