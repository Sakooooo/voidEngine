#ifndef ENGINE_H
#define ENGINE_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

class Engine {
public:
    // I wonder if this could be an array later
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool initalized;
    Engine();
    ~Engine();
};

#endif
