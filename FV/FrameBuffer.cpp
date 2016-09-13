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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void FrameBuffer::SetClearColor(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void FrameBuffer::Clear(ClearBit bit)
{
    glClear(bit);
}
