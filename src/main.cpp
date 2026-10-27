#include "camera.h"
#include "engine.h"
#include "gui.h"
#include "mind.h"
#include "script.h"
#include "world.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <vector>

int main() {
  SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

  Engine engine;

  if (!engine.initialized)
    return 1;

  auto entity = engine.world->create_entity();
  engine.world->add_component<Transform2D>(
      entity,
      Transform2D{.pos = Vector2{.x = 0.0f, .y = 0.0f},
		  .width = 100.0f,
		  .height = 100.0f});

  // TestSystem mySystem{};

  // engine.systems->add(&mySystem);

  Uint64 NOW = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;
  double deltaTime = 0;

  bool running = true;

  while (running) {
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();

    deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

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

    engine.tick();

    SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(engine.renderer);

    SDL_RenderPresent(engine.renderer);
  }

  return 0;
}
