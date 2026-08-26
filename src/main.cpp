#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "engine.h"
#include <cstdio>
#include <SDL3/SDL.h>
#include <vector>

extern "C" {
    #include <lua.h>
}

int main() {
  SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

  if (!SDL_Init(SDL_INIT_VIDEO)) {
      printf("Failed to initalize SDL Video! %s\n", SDL_GetError());
      return 1;
  }

  Engine engine;

  if (!engine.initalized)
      return 1;

  bool running = true;

  const std::vector<SDL_Vertex> verts = {
        { SDL_FPoint{ 400, 150}, SDL_FColor {255, 0, 0, 255 }, SDL_FPoint {0, 0}, },
        { SDL_FPoint{ 200, 450}, SDL_FColor {0, 0, 255, 255 }, SDL_FPoint {0, 0}, },
        { SDL_FPoint{ 600, 450}, SDL_FColor {0, 255, 0, 255 }, SDL_FPoint {0, 0}, },
  };

  while (running) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
          if (event.type == SDL_EVENT_QUIT) {
              running = false;
          }
          if (event.type == SDL_EVENT_KEY_DOWN) {
              if (event.key.key == SDLK_ESCAPE)
                  running = false;
          }
      }
      SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(engine.renderer);

      SDL_RenderGeometry(engine.renderer, nullptr, verts.data(), verts.size(), nullptr, 0);

      SDL_RenderPresent(engine.renderer);
  }

  SDL_Quit();
  return 0;
}
