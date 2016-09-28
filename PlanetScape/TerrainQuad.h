#ifndef PS_TERRAINQUAD_H
#define PS_TERRAINQUAD_H

#include "Quad.h"
#include "FV/Mesh.h"
#include <functional>

namespace PlanetScape
{

//!
//! \brief A quad that contains terrain information (elevation, normals, etc.)
//!
class TerrainQuad : public Quad
{
public:
    TerrainQuad(std::shared_ptr<Quad> parent = nullptr);

    //! \brief The tile mesh, rendered for each terrain tile.
    static std::shared_ptr<FV::Mesh<glm::vec2>> s_quadMesh;

    //! \brief Creates the tile mesh. Must be called from the OpenGL thread.
    static void CreateTileMesh();

    //! \brief Renders the tile mesh. Does not change the OpenGL state.
    static void RenderTile();

    //! The number of quads on a tile's edge.
    static constexpr int TILE_SIZE = 24;

    //! Subdivides the terrain quad.
    virtual void Subdivide();

    typedef std::function<void(std::shared_ptr<TerrainQuad>)> TerrainCallback;

    //!
    //! Recursively parses the terrain quadtree, calling the callback for each
    //! tile.
    //! \param callback The callback to call.
    //!
    virtual void Parse(TerrainCallback callback);

    //! Returns the center of the quad.
    inline glm::vec2 GetCenter() const {
        return m_center;
    }

    //! Returns the scale of the quad.
    inline float GetScale() const {
        return m_scale;
    }

protected:
    friend class TerrainQuad;

    glm::vec2 m_center;
    float m_scale = 1.0f;
};

}

#endif // PS_TERRAINQUAD_H
