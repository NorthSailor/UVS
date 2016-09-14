#include "UVSWindow.h"
#include <FV/Common.h>
#include <FV/Model.h>
#include <thread>
#include <assert.h>
#include <chrono>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;
using namespace FV;
using namespace std::chrono;
using namespace std::this_thread;

UVSWindow::UVSWindow(bool fullscreen) :
    SDLWindow("Universal Vehicle Simulator - Pre-alpha", fullscreen)
{
}

void UVSWindow::Initialize()
{
    glClearColor(0.6f, 0.5f, 0.4f, 1.0f);
    m_f16 = make_shared<Model>("Resources/F-16/F-16C_FightingFalcon.obj", m_loader);
    auto start = high_resolution_clock::now();
    m_loader.FinishLoading();
    auto end = high_resolution_clock::now();
    Log(INFO, "Total model loading time %.3fs.",
        0.001f * duration_cast<milliseconds>(end - start).count());
    m_program = m_loader.LoadProgram("model.glsl");
    m_program->Use();
    m_renderingUniforms.transform = m_program->GetUniform("ModelTransform");
    m_renderingUniforms.diffuseSampler = m_program->GetUniform("diffuseSampler");

    glm::mat4 view = glm::lookAt(glm::vec3(2, 12, 2),
                                 glm::vec3(0, 0, -1),
                                 glm::vec3(0, 0, 1));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            1.5f, 1.0f, 10000.0f);

    m_program->GetUniform("ViewProjection").Set(projection * view);

    m_colorTex = make_shared<Texture>(Texture::TEX_2D);
    m_colorTex->SetImage2D(GetWidth(), GetHeight(),
                          nullptr, Texture::RGB, Texture::UBYTE, Texture::RGB);
    m_depthStencilTex = make_shared<Texture>(Texture::TEX_2D);
    m_depthStencilTex->SetImage2D(GetWidth(), GetHeight(),
                                 nullptr, Texture::DEPTH24_STENCIL8,
                                 Texture::UINT24_8, Texture::DEPTH_STENCIL);
    m_fb.Bind();
    m_colorTex->Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTex->GetID(), 0);
    m_depthStencilTex->Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthStencilTex->GetID(), 0);
    FrameBuffer::RestoreDefault();
    m_invertProg = m_loader.LoadProgram("invert.glsl");
    m_invertProg->Use();
    m_texSampler = m_invertProg->GetUniform("tex");
    m_invertProg->GetUniform("uStep").Set(1.0f / GetWidth());
    m_invertProg->GetUniform("vStep").Set(1.0f / GetHeight());
}

void UVSWindow::UpdateLoop()
{
    while (!m_shouldQuit) {
        sleep_for(milliseconds(50));
    }
}

void UVSWindow::Render(double, float)
{
    static float totalTime = 0.0f;
    static int count = 0;
    auto start = high_resolution_clock::now();

    m_fb.Bind();
    glEnable(GL_DEPTH_TEST);
    m_fb.SetClearColor(glm::vec4(0.5f, 0.5f, 1.0f, 1.0f));
    assert(m_fb.IsComplete());
    m_fb.Clear();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_program->Use();
    m_f16->Draw(m_program, m_renderingUniforms);
    glDisable(GL_BLEND);

    FrameBuffer fb(true);
    glDisable(GL_DEPTH_TEST);
    fb.Bind(FrameBuffer::DRAW);
    fb.SetClearColor( { 0.5f, 0.4f, 0.3f, 1 } );
    fb.Clear();
    m_invertProg->Use();
    m_texSampler.Set(m_colorTex, 0);
    fb.DrawQuad();

    auto end = high_resolution_clock::now();

    totalTime += duration_cast<microseconds>(end - start).count() * 0.001f;
    count++;
    if (count == 10) {
        float average = totalTime / count;
        // Log(INFO, "Average frame time: %.3f", average);
        count = 0;
        totalTime = 0;
    }

    assert(glGetError() == 0);
}
