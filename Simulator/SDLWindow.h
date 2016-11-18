#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include <SDL2/SDL.h>
#include <string>
#include <thread>

class SDLWindow
{
public:
    SDLWindow(std::string title, bool fullscreen = false);
    ~SDLWindow();

    void Show();
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

protected:
    SDL_Window *m_window;
    bool m_shouldQuit;
    int m_width, m_height; // In pixels!

    virtual void UpdateLoop();

    virtual void HandleSDLEvent(SDL_Event *e);
    virtual void Initialize();
    virtual void Render(double t, float dt);
    virtual void Update(double t, float dt);
};

#endif // SDLWINDOW_H
