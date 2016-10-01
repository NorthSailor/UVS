#ifndef Universe_h
#define Universe_h

#include "Planet.h"
#include "glm/gtc/quaternion.hpp"
#include <vector>
#include <memory>
#include "FV/Framebuffer.h"
#include "FV/ObjectLoader.h"

namespace PlanetScape {
    class Universe {
    public:
        Universe(FV::ObjectLoader &objLoader);
        
        inline void SetCameraPos(const SpacePosition& cameraPos) {
            m_cameraPos = cameraPos;
        }
        
        inline const SpacePosition& GetCameraPos() const {
            return m_cameraPos;
        }
        
        inline void SetCameraOrientation(const glm::quat& cameraOrientation) {
            m_cameraOrientation = cameraOrientation;
        }
        
        inline const glm::quat& GetCameraOrientation() const {
            return m_cameraOrientation;
        }
        
        inline void SetProjectionMatrix(const glm::dmat4& pm) {
            m_mProjection = pm;
        }
        
        inline const glm::dmat4& GetProjectionMatrix() const {
            return m_mProjection;
        }
        
        void Render(FV::FrameBuffer &fb);
        
    protected:
        FV::ObjectLoader &m_objLoader;
        std::vector<std::shared_ptr<Planet>> m_planets;
        SpacePosition m_cameraPos;
        glm::quat m_cameraOrientation;
        glm::dmat4 m_mProjection;
        
        std::shared_ptr<FV::Texture> m_skybox;
        std::shared_ptr<FV::Program> m_progSkybox;
        FV::Program::Uniform m_umInverseTransform;
        glm::quat m_qZtoY;
        
        void LoadSkybox();
        void DrawSkybox(FV::FrameBuffer &fb);
    };
}

#endif /* Universe_h */
