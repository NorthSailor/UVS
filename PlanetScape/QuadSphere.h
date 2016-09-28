#ifndef SphericalDeformation_h
#define SphericalDeformation_h

#include "glm/vec3.hpp"

namespace PlanetScape {
    
    class QuadSphere {
        template <typename T>
        virtual glm::tvec3<T> DeformX(glm::tvec3<T> local) {
            return (m_radius + x) * normalize(glm::tvec3(0.0f, local.y, local.z));
        }
        
        template <typename T>
        virtual glm::tvec3<T> DeformY(glm::tvec3<T> local) {
            return (m_radius + y) * normalize(glm::tvec3(local.x, 0.0f, local.z));
        }
        
        template <typename T>
        virtual glm::tvec3<T> DeformZ(glm::tvec3<T> local) {
            return (m_radius + z) * normalize(glm::tvec3(local.x, local.y, 0.0f));
        }
        
    protected:
        double m_radius;
    };
    
}

#endif /* SphericalDeformation_h */
