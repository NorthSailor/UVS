#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <SDL2/SDL.h>
#include <string>
#include <thread>

class SDLWindow
{
public:
    SDLWindow(std::string title, bool fullscreen = false);

    void Show();

protected:
    SDL_Window *m_window;
    bool m_shouldQuit;

    virtual void UpdateLoop();

    virtual void HandleSDLEvent(SDL_Event *e);
    virtual void Initialize();
    virtual void Render(double t, float dt);
    virtual void Update(double t, float dt);
};

#endif // SDLWINDOW_H
