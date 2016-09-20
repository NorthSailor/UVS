#ifndef UVSWINDOW_H
#define UVSWINDOW_H

#include "SDLWindow.h"
#include <FV/ObjectLoader.h>
#include <FV/Model.h>
#include <FV/FrameBuffer.h>
#include <FV/Texture.h>
#include <PlanetScape/TerrainQuad.h>

class UVSWindow : public SDLWindow
{
public:
    UVSWindow(bool fullscreen = false);
    ~UVSWindow();

protected:
    virtual void UpdateLoop();
    virtual void Initialize();
    virtual void Render(double t, float dt);
    virtual void HandleSDLEvent(SDL_Event *e);

    std::shared_ptr<FV::Program> m_postProg;
    FV::Program::Uniform m_texSampler;

    FV::ObjectLoader m_loader;

    FV::FrameBuffer m_fb;
    FV::FrameBuffer::RenderBuffer m_colorBuffer;
    FV::FrameBuffer::RenderBuffer m_depthBuffer;

    FV::FrameBuffer m_fb2;
    std::shared_ptr<FV::Texture> m_colorTex;

    std::shared_ptr<FV::Program> m_terrainProg;
    std::shared_ptr<PlanetScape::TerrainQuad> m_rootQuad;
};

#endif // UVSWINDOW_H
