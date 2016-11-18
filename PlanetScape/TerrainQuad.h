#ifndef PS_TERRAINQUAD_H
#define PS_TERRAINQUAD_H

#include "Quad.h"
#include "FV/Mesh.h"
#include <functional>

namespace PlanetScape
{
    enum CubeFace {
        XPLUS = 0,
        XMINUS = 1,
        YPLUS = 2,
        YMINUS = 3,
        ZPLUS = 4,
        ZMINUS = 5
    };
    
    struct LogicalPosition {
        CubeFace face;
        int level;
        glm::dvec2 offset;
    };
    
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

    const LogicalPosition& GetLogicalPosition() const {
        return m_logicalPos;
    }
    
    void SetLogicalPosition(const LogicalPosition& lpos) {
        m_logicalPos = lpos;
    }
    
protected:
    friend class TerrainQuad;
    LogicalPosition m_logicalPos;
};

}

#endif // PS_TERRAINQUAD_H
