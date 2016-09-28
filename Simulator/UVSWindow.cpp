#include "UVSWindow.h"
#include "FV/Common.h"
#include "FV/Model.h"
#include <thread>
#include <assert.h>
#include <chrono>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
using namespace std;
using namespace FV;
using namespace glm;
namespace PS = PlanetScape;
using namespace std::chrono;
using namespace std::this_thread;

UVSWindow::UVSWindow(bool fullscreen) :
    SDLWindow("Universal Vehicle Simulator - Pre-alpha", fullscreen),
    m_earth(PS::SpacePosition(), 6000.0)
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
    m_camera = glm::lookAt(vec3(12, 7, 10), vec3(), vec3(0, 0, 1));
    m_projection = glm::perspectiveFov(45.0f,
        (float)GetWidth(), (float)GetHeight(), 1.0f, 1000000.0f);
    m_terrainProg->Use();
    m_uTransform = m_terrainProg->GetUniform("transform");

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

    static bool drawWireframe = true;
    if (drawWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    static bool depthTest = true;
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    m_terrainProg->Use();

    // Render the terrain
    auto renderTileCallback = [this] (shared_ptr<PS::TerrainQuad> quad) {
        vec3 center = vec3(quad->GetCenter().x, quad->GetCenter().y, quad->GetScale() * 5.0f);
        mat4 scale = glm::scale(mat4(), vec3(quad->GetScale() * 1.01f, quad->GetScale() * 1.01f, 1.0f));
        mat4 translate = glm::translate(mat4(), center);
        m_uTransform.Set(m_projection * m_camera * translate * scale);
        PS::TerrainQuad::RenderTile();
    };
    
    (void)renderTileCallback;

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

    static bool showFPS = true;

    if (showFPS) {
        ImGui::Begin("Performance");
        ImGui::Text("Average %.1f FPS (%.2f ms)", ImGui::GetIO().Framerate,
                1000.0f / ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Simulator")) {
            ImGui::MenuItem("Show performance", nullptr, &showFPS);
            ImGui::Separator();
            ImGui::MenuItem("Quit", "Alt + F4", &m_shouldQuit);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Display")) {
            ImGui::MenuItem("Draw wireframe", nullptr, &drawWireframe);
            ImGui::MenuItem("Test depth", nullptr, &depthTest);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    FrameBuffer::RestoreDefault();

    assert(glGetError() == 0);
}

void UVSWindow::HandleSDLEvent(SDL_Event *e)
{
    SDLWindow::HandleSDLEvent(e);
}
