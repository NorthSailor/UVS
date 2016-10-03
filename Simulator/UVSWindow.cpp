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
    m_universe(m_loader)
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

    PlanetScape::TerrainQuad::CreateTileMesh();
    
    m_universe.SetProjectionMatrix(glm::perspectiveFov(radians(45.0),
                                                       (double)GetWidth(),
                                                       (double)GetHeight(),
                                                       1.0, 1000000.0));
    SDL_SetRelativeMouseMode(SDL_TRUE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    
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
    static bool drawWireframe = false;
    if (drawWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    static bool depthTest = true;
    if (depthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    
    m_universe.Render(m_fb);
    
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
    switch (e->type) {
        case SDL_MOUSEMOTION:
        {
            if (!SDL_GetRelativeMouseMode())
                break;
            int xrel = e->motion.xrel;
            int yrel = e->motion.yrel;
            float vrot = -xrel * 0.003f;
            float hrot = -yrel * 0.003f;
            quat current = m_universe.GetCameraOrientation();
            vec3 hAxis = inverse(current) * vec3(1, 0, 0);
            vec3 vAxis = inverse(current) * vec3(0, 1, 0);
            current *= angleAxis(vrot, vAxis);
            current *= angleAxis(hrot, hAxis);
            m_universe.SetCameraOrientation(current);
            break;
        }
        case SDL_KEYUP:
            if (e->key.keysym.sym == SDLK_SPACE) {
                SDL_SetRelativeMouseMode(SDL_GetRelativeMouseMode() ? SDL_FALSE : SDL_TRUE);
            }
            break;
        default:
            SDLWindow::HandleSDLEvent(e);
    }
}
