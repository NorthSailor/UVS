#include "Planet.h"
#include "FV/Log.h"
using namespace FV;
using namespace PlanetScape;
using namespace glm;
using namespace std;

Planet::Planet(SpacePosition center, double radius) :
    m_center(center),
    m_deformation(radius),
    m_radius(radius)
{
    // Create the quads.
    m_quads.resize(6);
    LogicalPosition lpos = { ZPLUS, 0, vec2(0.0f, 0.0f)};
    for (shared_ptr<TerrainQuad> quad : m_quads) {
        static int i = 0;
        quad = make_shared<TerrainQuad>();
        lpos.face = static_cast<CubeFace>(i);
        quad->SetLogicalPosition(lpos);
    }
}
