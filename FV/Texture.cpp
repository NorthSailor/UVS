#include "Texture.h"
#include "Common.h"
#include "stbi/stb_image.h"
using namespace FV;
using namespace std;

Texture::Texture(Target target) :
    m_target(target),
    m_final(false)
{
    glGenTextures(1, &m_ID);
    assert(m_ID > 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}

void Texture::Bind(int unit)
{
    if (!m_final) {
        m_final = true; // Now Finalize() can call Bind().
        Finalize();
    }
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(m_target, m_ID);
    m_currentUnit = unit;
}

void Texture::ReleaseUnit()
{
    if (m_currentUnit > -1) {
        glActiveTexture(GL_TEXTURE0 + m_currentUnit);
        glBindTexture(m_target, 0);
        m_currentUnit = -1;
    } else {
        Log(WARNING, "Trying to free a unit twice.");
    }
}

void Texture::GenerateMipmaps() const
{
    glGenerateMipmap(m_target);
}

void Texture::Finalize()
{
    // Nothing to do for this class.
}

bool Texture::operator ==(const Texture& texture)
{
    return m_ID == texture.m_ID;
}
