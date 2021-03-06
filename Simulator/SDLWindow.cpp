#include "SDLWindow.h"
#include "FV/OpenGL.h"
#include "FV/Exception.h"
#include "FV/Log.h"
#include <chrono>
#include <thread>
#include <assert.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"
using namespace std;
using namespace std::chrono;
using namespace FV;

SDLWindow::SDLWindow(std::string title, bool fullscreen) :
    m_shouldQuit(false),
    m_width(1024),
    m_height(768)
{
    m_window = SDL_CreateWindow(title.c_str(),
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_width, m_height,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (m_window == nullptr)
        throw Exception("Failed to create an OpenGL window.");

    if (!SDL_GL_CreateContext(m_window))
        throw Exception("Failed to create an OpenGL context.");

    SDL_SetWindowFullscreen(m_window,
                            (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));

#ifndef MACOS
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK)
        throw Exception("Failed to initialize GLEW ");

    if (GLEW_ARB_clip_control) {
        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    } else {
        Log(WARNING, "\"ARB_clip_control\" extension not supported."
                     " Depth testing may fail.");
    }
#endif
    
    // Use an inverted depth buffer.
    glDepthRange(1.0, 0.0);
    
    if (glGetError() == GL_INVALID_ENUM)
        Log(WARNING, "OpenGL sent INVALID_ENUM.");

    if (!ImGui_ImplSdlGL3_Init(m_window))
        throw Exception("Failed to initialize ImGui.");
}

SDLWindow::~SDLWindow()
{
    ImGui_ImplSdlGL3_Shutdown();
}

void SDLWindow::Initialize()
{
    // Nothing to do in this base class.
}

void SDLWindow::Show()
{
    // In case the window went fullscreen.
    SDL_GetWindowSize(m_window, &m_width, &m_height);
    Initialize();

    glViewport(0, 0, GetWidth(), GetHeight());
    
    thread updateThread(&SDLWindow::UpdateLoop, this);
    SDL_Event e;
    SDL_GL_SetSwapInterval(1); // Enable VSync

    while (m_shouldQuit == false) {
        while (SDL_PollEvent(&e) != 0) {
            HandleSDLEvent(&e);
        }
        static auto lastTick = steady_clock::now();
        static auto firstTick = lastTick;

        auto currentTick = steady_clock::now();
        auto frameTime = duration_cast<milliseconds>(currentTick - lastTick);
        auto gameTime = duration_cast<milliseconds>(currentTick - firstTick);

        ImGui_ImplSdlGL3_NewFrame(m_window);

        Render((double)(0.001 * gameTime.count()),
               (float)(0.001f * frameTime.count()));

        ImGui::Render();

        lastTick = currentTick;
        SDL_GL_SwapWindow(m_window);
    }
    updateThread.join();
}

void SDLWindow::UpdateLoop()
{
}

void SDLWindow::HandleSDLEvent(SDL_Event *e)
{
    switch (e->type) {
    case SDL_QUIT:
        m_shouldQuit = true;
        break;
    }
    ImGui_ImplSdlGL3_ProcessEvent(e);
}

void SDLWindow::Render(double, float)
{
    // Nothing to render here.
}

void SDLWindow::Update(double, float)
{
    // Nothing to update.
}
