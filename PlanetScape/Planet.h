#ifndef Planet_hpp
#define Planet_hpp

#include "TerrainQuad.h"
#include "QuadSphere.h"

namespace PlanetScape {
    class Planet {
    public:
        //! \brief Creates a new planet.
        //! \param center The planet's center.
        //! \param radius The planet's radius in meters.
        Planet(SpacePosition center, double radius);
        
        const SpacePosition& GetCenter() const {
            return m_center;
        }
        
        const QuadSphere& GetDeformation() const {
            return m_deformation;
        }
        
        double GetRadius() const {
            return m_radius;
        }
        
        const std::vector<std::shared_ptr<TerrainQuad>> GetQuads() {
            return m_quads;
        }
        
    protected:
        std::vector<std::shared_ptr<TerrainQuad>> m_quads;
        SpacePosition m_center;
        QuadSphere m_deformation;
        double m_radius;
        
    };
}

#endif /* Planet_hpp */
