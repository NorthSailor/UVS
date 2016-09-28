#ifndef Planet_hpp
#define Planet_hpp

#include "TerrainQuad.h"

namespace PlanetScape {
    class Planet {
    public:
        enum QuadFace {
            XPLUS,
            XMINUS,
            YPLUS,
            YMINUS,
            ZPLUS,
            ZMINUS
        };
        
        //! \brief Creates a new planet.
        //! \param center The planet's center.
        //! \param radius The planet's radius in meters.
        Planet(SpacePosition center, double radius);
        
    protected:
        std::vector<std::shared_ptr<TerrainQuad>> m_quads;
        SpacePosition m_center;
        double m_radius;
        
    };
}

#endif /* Planet_hpp */
