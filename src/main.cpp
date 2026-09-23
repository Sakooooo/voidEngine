#include "engine.h"
#include "gui.h"
#include "mind.h"
#include "script.h"
#include "world.h"
#include <SDL3/SDL.h>
#include <cstdio>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <vector>

int main() {
  SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

  // Engine owns SDL_Init/SDL_Quit, the window, the renderer and the GUI, and
  // tears them down in the right order when it goes out of scope.
  Engine engine;

  if (!engine.initialized)
    return 1;

  EntityUI my_entity_gui{engine.gui_manager.get()};

  MyDebugUi debug_ui{engine.gui_manager.get()};

  debug_ui.entity_debug = &my_entity_gui;

  engine.gui_manager->AddPanel(&debug_ui);

  engine.gui_manager->AddPanel(&my_entity_gui);

  auto& world = World::get_instance();

  world.set_engine(engine);

  auto entity = world.createEntity();
  world.add_component<Transform>(entity, 200.0f, 400.0f);
  world.add_component<Color>(entity, 55, 155, 55, 255);
  world.add_component<Controllable>(entity, Mind{.controlled = true});

  auto secondEntity = world.createEntity();
  world.add_component<Transform>(secondEntity, 800.0f, 400.0f);
  world.add_component<Color>(secondEntity, 255, 155, 55, 255);
  world.add_component<Script>(secondEntity, "./scripts/test.lua");

  auto thirdEntity = world.createEntity();
  world.add_component<Transform>(thirdEntity, 200.0f, 800.0f);

  // test view()
  {
    if (auto comps = world.view<Transform, Color>(entity)) {
      printf("I got entity with transform and color\n");
      auto& [pos, color] = *comps;
      printf("Transform: %f, %f\n", pos.x, pos.y);
      printf("Color: %i %i %i %i\n", color.r, color.g, color.b, color.a);
    } else {
      printf("nah\n");
    }

    if (auto comps = world.view<Transform, Color>(secondEntity)) {
      printf("I got entity with transform and color\n");
      auto& [pos, color] = *comps;
      printf("Transform: %f, %f\n", pos.x, pos.y);
      printf("Color: %i %i %i %i\n", color.r, color.g, color.b, color.a);
    } else {
      printf("nah\n");
    }
  }

  bool running = true;

  // SDL_FColor channels are floats in 0..1, not 0..255
  const std::vector<SDL_Vertex> verts = {
      {
	  SDL_FPoint{400, 150},
	  SDL_FColor{1.0f, 0.0f, 0.0f, 1.0f},
	  SDL_FPoint{0, 0},
      },
      {
	  SDL_FPoint{200, 450},
	  SDL_FColor{0.0f, 0.0f, 1.0f, 1.0f},
	  SDL_FPoint{0, 0},
      },
      {
	  SDL_FPoint{600, 450},
	  SDL_FColor{0.0f, 1.0f, 0.0f, 1.0f},
	  SDL_FPoint{0, 0},
      },
  };

  const ImGuiIO& io = *engine.gui_manager->io;

  Uint64 NOW = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;
  double deltaTime = 0;

  while (running) {
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();

    deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);
      if (event.type == SDL_EVENT_QUIT) {
	running = false;
      }

      // When an ImGui widget (e.g. a text field) has keyboard focus, don't
      // also treat the keys as game input.
      if (event.type == SDL_EVENT_KEY_DOWN && !io.WantCaptureKeyboard) {
	if (event.key.key == SDLK_ESCAPE)
	  running = false;

	if (event.key.key == SDLK_F7)
	  debug_ui.visible = !debug_ui.visible;

	mindSystem(engine.keyboard_state);
	ControllableSystem();
      }
    }

    scriptTickSystem(&engine, deltaTime);

    engine.gui_manager->RenderPanels();
    SDL_SetRenderScale(engine.renderer, io.DisplayFramebufferScale.x,
		       io.DisplayFramebufferScale.y);
    SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(engine.renderer);

    SDL_RenderGeometry(engine.renderer, nullptr, verts.data(),
		       static_cast<int>(verts.size()), nullptr, 0);

    mySystem(engine.renderer);
    funnyRainbowSystem(deltaTime);

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(),
					  engine.renderer);

    SDL_RenderPresent(engine.renderer);
  }

  return 0;
}
