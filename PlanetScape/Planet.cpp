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
    m_quads.reserve(6);
    LogicalPosition lpos = { ZPLUS, 0, vec2(0.0f, 0.0f)};
    for (int i = 0; i < 6; i++) {
        shared_ptr<TerrainQuad> q = make_shared<TerrainQuad>();
        lpos.face = static_cast<CubeFace>(i);
        q->SetLogicalPosition(lpos);
        m_quads.push_back(q);
    }
}
