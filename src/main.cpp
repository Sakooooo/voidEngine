#include "camera.h"
#include "engine.h"
#include "gui.h"
#include "guis/guis.h"
#include "player_controller.h"
#include "rectangle.h"
#include "script.h"
#include "world.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <spdlog/spdlog.h>
#include <vector>

int main() {
  spdlog::default_logger()->set_level(
      spdlog::level::trace); // TODO: make this only happen on debug builds
  spdlog::info("voidengine 0.0.1"); // TODO: make this automated or something
  SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

  Engine::CEngine engine;

  if (!engine.initialized)
    return 1;

  auto entity = engine.world->create_entity();
  engine.world->add_component<Transform2D>(
      entity,
      Transform2D{.pos = Vector2{.x = 0.0f, .y = 0.0f},
                  .width = 100.0f,
                  .height = 100.0f});
  // engine.world->add_component<PlayerController>(entity, PlayerController{});
  engine.world->add_component<Color>(entity,
                                     Color{.r = 255, .g = 0, .b = 0, .a = 255});
  engine.world->add_component<Rect>(entity, Rect{});
  engine.world->add_component<Camera2D>(
      entity,
      Camera2D{.current = true, .follow = engine.world->make_ref(entity)});
  engine.world->add_component<LuauScript>(entity,
                                          LuauScript{.path = "test.luau"});

  auto entity2 = engine.world->create_entity();
  engine.world->add_component<Transform2D>(
      entity2,
      Transform2D{.pos = Vector2{.x = 100.0f, .y = 0.0f},
                  .width = 100.0f,
                  .height = 100.0f});
  engine.world->add_component<Color>(
      entity2, Color{.r = 255, .g = 255, .b = 0, .a = 255});
  engine.world->add_component<Rect>(entity2, Rect{});

  DebugGui debug_gui;
  engine.gui_manager->add_gui(&debug_gui);

  TreeGui tree_gui;
  engine.gui_manager->add_gui(&tree_gui);

  // TestSystem mySystem{};

  // engine.systems->add(&mySystem);

  CLuauScriptSystem script_system{};
  engine.systems->add(&script_system);

  CPlayerControllerSystem controller_system{};
  engine.systems->add(&controller_system);

  CCamera2DSystem camera_system{};
  engine.systems->add(&camera_system);

  CRectSystem rectangle_system{};
  engine.systems->add(&rectangle_system);

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
      engine.gui_manager->event(&event);
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }

      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE)
          running = false;
      }
    }

    engine.gui_manager->begin();

    engine.tick(deltaTime);

    engine.gui_manager->render();

    SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(engine.renderer);

    engine.draw();

    engine.gui_manager->end();

    SDL_RenderPresent(engine.renderer);
  }

  return 0;
}
