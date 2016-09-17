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

    m_program = m_loader.LoadProgram("model.glsl");
    m_program->Use();
    m_renderingUniforms.transform = m_program->GetUniform("ModelTransform");
    m_renderingUniforms.diffuseSampler = m_program->GetUniform("diffuseSampler");

    glm::mat4 view = glm::lookAt(glm::vec3(12, 22, 2),
                                 glm::vec3(0, 0, -1),
                                 glm::vec3(0, 0, 1));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            1.5f, 1.0f, 100000000.0f);

    m_program->GetUniform("ViewProjection").Set(projection * view);

    m_fb.Bind();
    m_colorBuffer.Bind();
    m_colorBuffer.SetStorageMultisample(GetWidth(), GetHeight(), 8, Texture::RGBA);
    m_fb.Attach(FrameBuffer::COLOR0, m_colorBuffer);
    m_depthBuffer.Bind();
    m_depthBuffer.SetStorageMultisample(GetWidth(), GetHeight(), 8, Texture::DEPTH32F);
    m_fb.Attach(FrameBuffer::DEPTH, m_depthBuffer);

    m_fb2.Bind();
    m_colorTex = make_shared<Texture>(Texture::TEX_2D);
    m_colorTex->SetImage2D(GetWidth(), GetHeight(), nullptr);
    m_colorTex->SetFiltering(Texture::NEAREST, Texture::NEAREST);
    m_fb2.Attach(FrameBuffer::COLOR0, m_colorTex);

    FrameBuffer::RestoreDefault();
    m_invertProg = m_loader.LoadProgram("invert.glsl");
    m_invertProg->Use();
    m_texSampler = m_invertProg->GetUniform("tex");
    m_invertProg->GetUniform("uStep").Set(1.0f / GetWidth());
    m_invertProg->GetUniform("vStep").Set(1.0f / GetHeight());

    m_loader.FinishLoading();
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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    m_fb.SetClearColor(glm::vec4(0.5f, 0.5f, 1.0f, 1.0f));
    assert(m_fb.IsComplete());
    m_fb.Clear();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_program->Use();
    m_f16->Draw(m_program, m_renderingUniforms);
    glDisable(GL_BLEND);
    m_fb.Bind(FrameBuffer::READ);

    m_fb2.Bind(FrameBuffer::DRAW);
    m_fb2.SetClearColor( { 1, 0, 0, 1 });
    m_fb2.Clear();
    FrameBuffer::Blit(0, 0, GetWidth(), GetHeight(), 0, 0, GetWidth(), GetHeight());

    FrameBuffer fb(true);
    glDisable(GL_DEPTH_TEST);
    fb.Bind();
    fb.SetClearColor( { 0.5f, 0.4f, 0.3f, 1 } );
    fb.Clear();
    m_invertProg->Use();
    m_texSampler.Set(m_colorTex, 0);
    fb.DrawQuad();

    auto end = high_resolution_clock::now();

    totalTime += duration_cast<microseconds>(end - start).count();
    count++;
    if (count == 100) {
        float average = totalTime / count;
        Log(INFO, "Average frame time: %.3f milliseconds.", average * 0.001f);
        count = 0;
        totalTime = 0;
    }

    assert(glGetError() == 0);
}
