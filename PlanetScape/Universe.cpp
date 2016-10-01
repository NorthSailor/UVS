#include "Universe.h"
using namespace PlanetScape;
using namespace FV;
using namespace std;
using namespace glm;

Universe::Universe(ObjectLoader &objLoader) : m_objLoader(objLoader)
{
    // Converts from a Z-up coordinate system to a Y-up coordinate system.
    m_qZtoY = angleAxis(radians(90.0f), vec3(0, 1, 0)) *
              angleAxis(radians(90.0f), vec3(-1, 0, 0));
    LoadSkybox();
}

void Universe::LoadSkybox()
{
    assert(m_skybox == nullptr);
    m_skybox = make_shared<Texture>(Texture::TEX_CUBE_MAP);
    vector<string> faces;
    faces.reserve(6);
    faces.push_back("PlanetScape/Skybox/XPlus.png");
    faces.push_back("PlanetScape/Skybox/XMinus.png");
    faces.push_back("PlanetScape/Skybox/YPlus.png");
    faces.push_back("PlanetScape/Skybox/YMinus.png");
    faces.push_back("PlanetScape/Skybox/ZPlus.png");
    faces.push_back("PlanetScape/Skybox/ZMinus.png");
    m_skybox->LoadCubemap(faces);
    m_skybox->SetFiltering(Texture::LINEAR, Texture::LINEAR);
    
    m_progSkybox = m_objLoader.LoadProgram("PlanetScape/skybox.glsl");
    m_progSkybox->Use();
    m_progSkybox->GetUniform("skybox").Set(m_skybox, 0);
    m_umInverseTransform = m_progSkybox->GetUniform("invTransform");
}

void Universe::DrawSkybox(FrameBuffer &fb)
{
    // Find the inverse matrices (yes, it could be cached).
    mat4 invProjection = inverse(m_mProjection);
    mat4 invCamera = mat4_cast(inverse(m_cameraOrientation * m_qZtoY));
    m_progSkybox->Use();
    m_umInverseTransform.Set(invCamera * invProjection);
    fb.DrawQuad();
    fb.Clear(FrameBuffer::DEPTH_BIT);
}

void Universe::Render(FrameBuffer &fb)
{
    fb.Bind(FrameBuffer::DRAW);
    fb.Clear(FrameBuffer::COLOR_DEPTH_BIT);
    DrawSkybox(fb);
}
