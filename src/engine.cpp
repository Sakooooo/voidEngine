#include "engine.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <cstdio>

Engine::Engine() {
    if (!SDL_CreateWindowAndRenderer("voidEngine", 800, 600, NULL, &window, &renderer)) {
        printf("Failed to create Window and Renderer! %s", SDL_GetError());
        initalized = false;
    } else {
        initalized = true;
    }
}

Engine::~Engine() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
