#include "Universe.h"
using namespace PlanetScape;
using namespace FV;
using namespace std;

Universe::Universe()
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
}
