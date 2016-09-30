#include "Universe.h"
using namespace PlanetScape;
using namespace FV;
using namespace std;
using namespace glm;

Universe::Universe(ObjectLoader &objLoader) : m_objLoader(objLoader)
{
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
    
    m_progSkybox = m_objLoader.LoadProgram("PlanetScape/skybox.glsl");
    m_progSkybox->Use();
    m_progSkybox->GetUniform("skybox").Set(m_skybox, 0);
    // m_umInverseProjection = m_progSkybox->GetUniform("invProjection");
    m_umInverseCamera = m_progSkybox->GetUniform("invCamera");
}

void Universe::DrawSkybox(FrameBuffer &fb)
{
    m_cameraOrientation = glm::angleAxis(90.0f, vec3(1.0f, 0.0f, 0.0f));
    // Find the inverse matrices.
    // mat4 invProj = inverse(m_mProjection);
    //mat4 invCamera = mat4_cast(inverse(m_cameraOrientation));
    m_progSkybox->Use();
    // m_umInverseProjection.Set(invProj);
    m_umInverseCamera.Set(glm::mat4());
    fb.DrawQuad();
}

void Universe::Render(FrameBuffer &fb)
{
    fb.Bind(FrameBuffer::DRAW);
    fb.Clear(FrameBuffer::COLOR_DEPTH_BIT);
    DrawSkybox(fb);
}
