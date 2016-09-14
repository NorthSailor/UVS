#ifndef UVSWINDOW_H
#define UVSWINDOW_H

#include "SDLWindow.h"
#include <FV/ObjectLoader.h>
#include <FV/Model.h>
#include <FV/FrameBuffer.h>
#include <FV/Texture.h>

class UVSWindow : public SDLWindow
{
public:
    UVSWindow(bool fullscreen = false);


protected:
    virtual void UpdateLoop();
    virtual void Initialize();
    virtual void Render(double t, float dt);

    std::shared_ptr<FV::Model> m_f16;
    FV::Model::RenderingUniforms m_renderingUniforms;
    std::shared_ptr<FV::Program> m_program;
    std::shared_ptr<FV::Program> m_invertProg;
    FV::Program::Uniform m_texSampler;

    FV::ObjectLoader m_loader;

    FV::FrameBuffer m_fb;
    std::shared_ptr<FV::Texture> m_colorTex;
    std::shared_ptr<FV::Texture> m_depthStencilTex;
};

#endif // UVSWINDOW_H
