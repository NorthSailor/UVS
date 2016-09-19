#include "UVSWindow.h"
#include <FV/Common.h>
#include <FV/Model.h>
#include <thread>
#include <assert.h>
#include <chrono>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <imgui/imgui.h>
using namespace std;
using namespace FV;
using namespace glm;
using namespace std::chrono;
using namespace std::this_thread;

UVSWindow::UVSWindow(bool fullscreen) :
    SDLWindow("Universal Vehicle Simulator - Pre-alpha", fullscreen)
{
}

UVSWindow::~UVSWindow()
{
}

void UVSWindow::Initialize()
{
    FrameBuffer().SetClearColor(vec4(0, 0, 0, 1));

    m_fb.Bind();
    m_colorBuffer.Bind();
    m_colorBuffer.SetStorageMultisample(GetWidth(), GetHeight(), 4, Texture::RGBA);
    m_fb.Attach(FrameBuffer::COLOR0, m_colorBuffer);
    m_depthBuffer.Bind();
    m_depthBuffer.SetStorageMultisample(GetWidth(), GetHeight(), 4, Texture::DEPTH32F);
    m_fb.Attach(FrameBuffer::DEPTH, m_depthBuffer);

    m_fb2.Bind();
    m_colorTex = make_shared<Texture>(Texture::TEX_2D);
    m_colorTex->SetImage2D(GetWidth(), GetHeight(), nullptr);
    m_colorTex->SetFiltering(Texture::NEAREST, Texture::NEAREST);
    m_fb2.Attach(FrameBuffer::COLOR0, m_colorTex);

    FrameBuffer::RestoreDefault();
    m_postProg = m_loader.LoadProgram("post.glsl");
    m_postProg->Use();
    m_texSampler = m_postProg->GetUniform("tex");
    // m_postProg->GetUniform("uStep").Set(1.0f / GetWidth());
    // m_postProg->GetUniform("vStep").Set(1.0f / GetHeight());

    m_terrainProg = m_loader.LoadProgram("PlanetScape/terrain.glsl");
    glm::mat4 transform = glm::lookAt(vec3(30, 10, 10), vec3(), vec3(0, 0, 1));
    glm::mat4 proj = glm::perspectiveFov(45.0f, (float)GetWidth(), (float)GetHeight(), 1.0f, 1000000.0f);
    m_terrainProg->Use();
    m_terrainProg->GetUniform("transform").Set(proj * transform);

    PlanetScape::TerrainQuad::CreateTileMesh();

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
    m_fb.Bind();
    glEnable(GL_DEPTH_TEST);
    m_fb.SetClearColor(vec4(0.1f, 0.1f, 0.05f, 1));
    m_fb.Clear();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    static bool drawWireframe = false;
    ImGui::Checkbox("Wireframe", &drawWireframe);
    if (drawWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_terrainProg->Use();
    PlanetScape::TerrainQuad::RenderTile();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_BLEND);
    m_fb.Bind(FrameBuffer::READ);

    m_fb2.Bind(FrameBuffer::DRAW);
    FrameBuffer::Blit(0, 0, GetWidth(), GetHeight(), 0, 0, GetWidth(), GetHeight());

    FrameBuffer fb(true);
    glDisable(GL_DEPTH_TEST);
    fb.Bind();
    m_postProg->Use();
    m_texSampler.Set(m_colorTex, 0);
    fb.DrawQuad();

    ImGui::Text("Average %.1f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    FrameBuffer::RestoreDefault();

    assert(glGetError() == 0);
}

void UVSWindow::HandleSDLEvent(SDL_Event *e)
{
    SDLWindow::HandleSDLEvent(e);
}
