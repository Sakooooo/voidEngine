#include "engine.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <cstdio>

Engine::Engine() {
    printf("Creating engine...\n");
    if (!SDL_CreateWindowAndRenderer("voidEngine", 800, 600, NULL, &window, &renderer)) {
        printf("Failed to create Window and Renderer! %s\n", SDL_GetError());
        initalized = false;
    } else {
        initalized = true;
    }
}

Engine::~Engine() {
    printf("Destroying engine...\n");
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
