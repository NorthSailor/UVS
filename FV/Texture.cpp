#include "Texture.h"
#include "Common.h"
#include "stbi/stb_image.h"
using namespace FV;
using namespace std;

Texture::Texture(Target target) :
    m_target(target),
    m_final(false),
    m_hasMipmaps(false)
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

bool Texture::operator ==(const Texture& texture)
{
    return m_ID == texture.m_ID;
}

void Texture::SetImage2DAsync(int width, int height,
                           void *data,
                           Format iformat, Type type, Format format)
{
    m_width = width;
    m_height = height;
    m_data = data;
    m_iformat = iformat;
    m_type = type;
    m_format = format;
    m_final = false;
}

void Texture::SetImage2D(int width, int height, void *data, Format iformat,
                           Type type, Format format)
{
    m_width = width;
    m_height = height;
    m_data = data;
    m_iformat = iformat;
    m_type = type;
    m_format = format;
    m_final = true;
    Bind(0);
    glTexImage2D(GL_TEXTURE_2D, 0, m_iformat, m_width, m_height, 0, m_format,
                 m_type, m_data);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Texture::LoadImage2D(std::string filename, Format format)
{
    assert(m_target == TEX_2D);
    int req_components = STBI_rgb;
    if (format == RGBA)
        req_components = STBI_rgb_alpha;
    else
        assert(format == RGB);

    int components;
    m_data = stbi_load(filename.c_str(), &m_width, &m_height,
                                 &components, req_components);
    assert(m_data != nullptr);
    m_format = format;
    m_iformat = format;
    m_type = UBYTE;
    m_final = false;
    m_hasMipmaps = true;
}

void Texture::SendToGPU()
{
    switch (m_target) {
    case TEX_2D:
        glTexImage2D(GL_TEXTURE_2D, 0, m_iformat, m_width, m_height, 0, m_format,
                     m_type, m_data);
        break;
    default:
        break;
    };

    free(m_data);
}

void Texture::Finalize()
{
    assert(m_data != nullptr); // LoadImage2D() hasn't been called.
    Bind(0);
    SendToGPU();
    if (m_hasMipmaps)
        GenerateMipmaps();
}
