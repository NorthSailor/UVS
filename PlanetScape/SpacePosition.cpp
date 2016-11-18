#include "SpacePosition.h"
#include <cmath>
using namespace PlanetScape;

SpacePosition::SpacePosition(UniverseComponent x, UniverseComponent y,
                             UniverseComponent z) :
    glm::tvec3<UniverseComponent>(x, y, z)
{
}
