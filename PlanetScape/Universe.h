#ifndef Universe_h
#define Universe_h

#include "Planet.h"
#include "glm/gtc/quaternion.hpp"
#include <vector>
#include <memory>
#include "FV/Texture.h"

namespace PlanetScape {
    class Universe {
    public:
        Universe();
        
        void SetCameraPos(const SpacePosition& cameraPos) {
            m_cameraPos = cameraPos;
        }
        
        inline const SpacePosition& GetCameraPos() const {
            return m_cameraPos;
        }
        
        void SetCameraOrientation(const glm::quat& cameraOrientation) {
            m_cameraOrientation = cameraOrientation;
        }
        
        inline const glm::quat& GetCameraOrientation() const {
            return m_cameraOrientation;
        }
        
    protected:
        std::vector<std::shared_ptr<Planet>> m_planets;
        SpacePosition m_cameraPos;
        glm::quat m_cameraOrientation;
        
        std::shared_ptr<FV::Texture> m_skybox;
        
        void LoadSkybox();
    };
}

#endif /* Universe_h */
