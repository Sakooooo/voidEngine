#include "engine.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL_error.h>
#include <SDL3_image/SDL_image.h>
#include <cstdio>

Engine::Engine() {
  // This is probably going to look incredibly ugly!
  // Whatever! I can always just fix it later :D

  printf("Creating engine...\n");
  if (!SDL_CreateWindowAndRenderer("voidEngine", 800, 600, SDL_WINDOW_RESIZABLE,
                                   &window, &renderer)) {
    printf("Failed to create Window and Renderer! %s\n", SDL_GetError());
    initalized = false;
  } else {
    printf("Created Window and Renderer.\n");

    // set app icon
    // :O this is cool
    // TODO: GENERATE THIS WITH CMAKE IT DOESNT WORK ON GCC OR
    // MSVC!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    static const unsigned char icon[] = {
#embed "../assets/icon.png"
    };

    static constexpr std::size_t icon_size = sizeof(icon);

    SDL_IOStream *icon_io = SDL_IOFromConstMem(icon, icon_size);
    SDL_Surface *icon_surface = IMG_Load_IO(icon_io, true); // closes io for you

    SDL_SetWindowIcon(window, icon_surface);
    SDL_DestroySurface(icon_surface); // cleanup

    gui_manager = new GuiManager(window, renderer);

    if (!gui_manager->initalized) {
      printf("Failed to initalize GuiManager.\n");
      initalized = false;
    } else {
      printf("GuiManager is ready.\n");
      initalized = true;
    }
  }
}

Engine::~Engine() {
  printf("Destroying engine...\n");
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
}
