#include "Planet.h"
#include "FV/Log.h"
using namespace FV;
using namespace PlanetScape;
using namespace glm;
using namespace std;

Planet::Planet(SpacePosition center, double radius) :
    m_center(center),
    m_radius(radius)
{
    // Create the quads.
    m_quads.resize(6);
    for (shared_ptr<TerrainQuad> quad : m_quads) {
        quad = make_shared<TerrainQuad>();
        Log(INFO, "Creating planet quad...");
    }
}
