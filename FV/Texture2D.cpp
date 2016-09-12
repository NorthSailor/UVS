#include "Texture2D.h"
#include "Common.h"
#include "stbi/stb_image.h"
using namespace FV;
using namespace std;

Texture2D::Texture2D() :
    Texture(TEX_2D)
{
}

void Texture2D::SetImage2D(int width, int height,
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

void Texture2D::LoadImage2D(std::string filename, Format format)
{
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
}

void Texture2D::SendToGPU()
{
    glTexImage2D(GL_TEXTURE_2D, 0, m_iformat, m_width, m_height, 0, m_format,
                 m_type, m_data);
    free(m_data);
}

void Texture2D::Finalize()
{
    assert(m_data != nullptr); // LoadImage2D() hasn't been called.
    Bind(0);
    SendToGPU();
    GenerateMipmaps();
}
