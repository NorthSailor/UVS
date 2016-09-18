#include "UVSWindow.h"
#include <SDL2/SDL.h>
#include <FV/Log.h>
#include <FV/Exception.h>
#include <PlanetScape/SpacePosition.h>
namespace PS = PlanetScape;
using namespace FV;

int real_main(int, char **)
{
    PS::SpacePosition pos = glm::vec3(-640.640, 6640.66, 4600.35);
    glm::dvec3 doublePos = pos;
    Log(INFO, "Magnitude: %f", doublePos.x);
    Log(INFO, "A space position is specified with %d bytes.", sizeof(PS::SpacePosition));

    /* if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        Log(ERROR, "Failed to initialize SDL.");
        Log(INFO, "Note that UVS needs a window system to run.");
        return 1;
    } else {
        Log(INFO, "SDL successfully initalized.");
    }

    // OpenGL 3.3 Core Profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    UVSWindow window(false);
    window.Show(); */

    return 0;
}

int main(int argc, char** argv)
{
    try {
        real_main(argc, argv);
    } catch (FV::Exception &e) {
        Log(ERROR, "Uncaught exception: %s", e.what());
    }
    return 0;
}

