#include "engine.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <cstdio>

Engine::Engine()
{
    // This is probably going to look incredibly ugly!
    // Whatever! I can always just fix it later :D

    printf("Creating engine...\n");
    if (!SDL_CreateWindowAndRenderer("voidEngine", 800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        printf("Failed to create Window and Renderer! %s\n", SDL_GetError());
        initalized = false;
    }
    else
    {
        printf("Created Window and Renderer.\n");

        gui_manager = new GuiManager(window, renderer);

        if (!gui_manager->initalized)
        {
            printf("Failed to initalize GuiManager.\n");
            initalized = false;
        }
        else
        {
            printf("GuiManager is ready.\n");
            initalized = true;
        }
    }
}

Engine::~Engine()
{
    printf("Destroying engine...\n");
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
