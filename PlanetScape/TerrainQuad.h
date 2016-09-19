#ifndef PS_TERRAINQUAD_H
#define PS_TERRAINQUAD_H

#include "Quad.h"
#include <FV/Mesh.h>

namespace PlanetScape
{

//!
//! \brief A quad that contains terrain information (elevation, normals, etc.)
//!
class TerrainQuad : public Quad
{
public:
    TerrainQuad();

    //! \brief The tile mesh, rendered for each terrain tile.
    static std::shared_ptr<FV::Mesh<glm::vec2>> s_quadMesh;

    //! \brief Creates the tile mesh. Must be called from the OpenGL thread.
    static void CreateTileMesh();

    //! \brief Renders the tile mesh. Does not change the OpenGL state.
    static void RenderTile();

    //! The number of quads on a tile's edge.
    static constexpr int TILE_SIZE = 32;
};

}

#endif // PS_TERRAINQUAD_H
