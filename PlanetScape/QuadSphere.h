#ifndef SphericalDeformation_h
#define SphericalDeformation_h

#include "glm/vec3.hpp"
#include "TerrainQuad.h"

namespace PlanetScape {
   
    class QuadSphere {
    public:
        template <typename T>
        glm::tvec3<T> DeformX(glm::tvec3<T> local) const {
            return (m_radius + local.x) * normalize(glm::tvec3<T>(m_radius, local.y, local.z));
        }
        
        template <typename T>
        glm::tvec3<T> DeformY(glm::tvec3<T> local) const {
            return (m_radius + local.y) * normalize(glm::tvec3<T>(local.x, m_radius, local.z));
        }
        
        template <typename T>
        glm::tvec3<T> DeformZ(glm::tvec3<T> local) const {
            return (m_radius + local.z) * normalize(glm::tvec3<T>(local.x, local.y, m_radius));
        }
        
        template <typename T>
        glm::tvec3<T> Deform(glm::tvec3<T> local) const {
            return m_radius * normalize(local);
        }
        
        QuadSphere(double radius) : m_radius(radius) { }
        
        template <typename T>
        inline T GetScalingFactor(int level) const {
            return (T)(2.0) * (T)m_radius / (T)TerrainQuad::TILE_SIZE / (T)(level + 1);
        }
        
        template <typename T>
        glm::tvec3<T> LogicalToLocal(const LogicalPosition &lp) const {
            T scalingFactor = 1.0f * m_radius;
            glm::tvec2<T> surface = glm::tvec2<T>(lp.offset) * scalingFactor;
            switch (lp.face) {
                case XPLUS:
                    return glm::tvec3<T>(m_radius, surface.x, surface.y);
                case XMINUS:
                    return glm::tvec3<T>(-m_radius, -surface.x, surface.y);
                case YPLUS:
                    return glm::tvec3<T>(-surface.x, m_radius, surface.y);
                case YMINUS:
                    return glm::tvec3<T>(surface.x, -m_radius, surface.y);
                case ZPLUS:
                    return glm::tvec3<T>(surface.x, surface.y, m_radius);
                case ZMINUS:
                    return glm::tvec3<T>(-surface.x, -surface.y, -m_radius);
            }
        }
        
        template <typename T>
        glm::tvec3<T> DeformLogical(const LogicalPosition &lp) const {
            return Deform(LogicalToLocal<T>(lp));
        }
        
    protected:
        double m_radius;
    };
    
}

#endif /* SphericalDeformation_h */
