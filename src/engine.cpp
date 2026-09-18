#include "engine.h"
#include "gui.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_log.h>
#include <SDL3_image/SDL_image.h>
#include <cstddef>
#include <lauxlib.h>
#include <lualib.h>

Engine::Engine() {
  SDL_Log("Creating engine...");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL Video! %s", SDL_GetError());
    return;
  }

  if (!SDL_CreateWindowAndRenderer("voidEngine", 800, 600, SDL_WINDOW_RESIZABLE,
				   &window, &renderer)) {
    SDL_Log("Failed to create Window and Renderer! %s", SDL_GetError());
    return;
  }
  SDL_Log("Created Window and Renderer.");

  // set app icon
  // :O this is cool
  // TODO: GENERATE THIS WITH CMAKE IT DOESNT WORK ON GCC OR MSVC!
  static const unsigned char icon[] = {
#embed "../assets/icon.png"
  };
  static constexpr std::size_t icon_size = sizeof(icon);

  SDL_IOStream* icon_io = SDL_IOFromConstMem(icon, icon_size);
  SDL_Surface* icon_surface = IMG_Load_IO(icon_io, true); // closes io for you
  if (icon_surface) {
    SDL_SetWindowIcon(window, icon_surface);
    SDL_DestroySurface(icon_surface);
  } else {
    SDL_Log("Failed to load window icon: %s", SDL_GetError());
  }

  gui_manager = std::make_unique<GuiManager>(window, renderer);
  if (!gui_manager->initialized) {
    SDL_Log("Failed to initialize GuiManager.");
    return;
  }
  SDL_Log("GuiManager is ready.");

  SDL_Log("Prepare lua");

  lua = luaL_newstate();

  if (!lua) {
    SDL_Log("lua failed to load");
    return;
  }

  luaL_openlibs(lua);

  initialized = true;
}

Engine::~Engine() {
  SDL_Log("Destroying engine...");
  if (lua)
    lua_close(lua);
  gui_manager.reset();
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
}
