#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_render.h"
#include "engine.h"
#include <cstdio>
#include <SDL3/SDL.h>

extern "C" {
    #include <lua.h>
}

int main() {
  SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

  if (!SDL_Init(SDL_INIT_VIDEO)) {
      printf("Failed to initalize SDL Video! %s", SDL_GetError());
      return 1;
  }

  Engine engine;

  if (!engine.initalized)
      return 1;

  bool running = true;

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
      SDL_RenderClear(engine.renderer);

      SDL_RenderPresent(engine.renderer);
  }

  SDL_Quit();
  return 0;
}
