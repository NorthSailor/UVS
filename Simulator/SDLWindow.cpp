#include "SDLWindow.h"
#include <GL/glew.h>
#include <FV/Exception.h>
#include <FV/Log.h>
#include <chrono>
#include <thread>
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

    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK)
        throw Exception("Failed to initialize GLEW ");

    if (glGetError() == GL_INVALID_ENUM)
        Log(WARNING, "OpenGL sent INVALID_ENUM.");

    glEnable(GL_DEPTH_TEST);
}

void SDLWindow::Initialize()
{
    // Nothing to do in this base class.
}

void SDLWindow::Show()
{
    Initialize();
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
        Render((double)(0.001 * gameTime.count()),
               (float)(0.001f * frameTime.count()));

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
}

void SDLWindow::Render(double, float)
{
    // Nothing to render here.
}

void SDLWindow::Update(double, float)
{
    // Nothing to update.
}
