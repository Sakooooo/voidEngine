#include "engine.h"
#include "gui.h"
#include "system.h"
#include "world.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <cstddef>
#include <cstdio>
#include <lua.h>
#include <lualib.h>
#include <memory>
#include <spdlog/spdlog.h>

static int c_function_test(lua_State* L) {
  printf("I just got called from lua bro");
  return 0;
}

namespace Engine {

CEngine::CEngine() {
  spdlog::info("Creating engine...");

  spdlog::trace("sdl video init");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Failed to initialize SDL Video! %s", SDL_GetError());
    return;
  }
  spdlog::trace("ok");

  spdlog::trace("sdl renderer and window");
  if (!SDL_CreateWindowAndRenderer("voidEngine", 800, 600, SDL_WINDOW_RESIZABLE,
				   &window, &renderer)) {
    SDL_Log("Failed to create Window and Renderer! %s", SDL_GetError());
    return;
  }
  SDL_SetRenderVSync(renderer, 1); // TODO: customiztable option
  spdlog::trace("ok");

  // set app icon
  // :O this is cool
  // TODO: GENERATE THIS WITH CMAKE IT DOESNT WORK ON GCC OR MSVC!
  static const unsigned char icon[] = {
#embed "../assets/icon.png"
  };
  static constexpr std::size_t icon_size = sizeof(icon);

  spdlog::trace("app icon");
  SDL_IOStream* icon_io = SDL_IOFromConstMem(icon, icon_size);
  SDL_Surface* icon_surface = IMG_Load_IO(icon_io, true); // closes io for you
  if (icon_surface) {
    SDL_SetWindowIcon(window, icon_surface);
    SDL_DestroySurface(icon_surface);
  } else {
    SDL_Log("Failed to load window icon: %s", SDL_GetError());
  }
  spdlog::trace("ok");

  spdlog::trace("setting up luau");

  lua = luaL_newstate();

  if (!lua) {
    SDL_Log("lua failed to load");
    return;
  }
  spdlog::trace("ok");

  lua_pushcfunction(lua, c_function_test, "myTestFunction");
  lua_setglobal(lua, "myFunction");

  static const struct luaL_Reg input_functions[] = {
      {
	  "is_pressed",
	  Engine::lua::input_is_pressed,
      },
      {nullptr, nullptr}};

  luaL_register(lua, "input", input_functions);
  lua_pop(lua, 1);

  static const struct luaL_Reg vector2_functions[] = {{"new", Vector2::lua_new},
						      {nullptr, nullptr}};

  luaL_register(lua, "Vector2", vector2_functions);

  lua_pop(lua, 1);

  luaL_newmetatable(lua, "Vector2");
  lua_pushcfunction(lua, Vector2::lua_index, "vector2_index");
  lua_setfield(lua, -2, "__index");
  lua_pop(lua, 1);

  luaL_openlibs(lua);

  spdlog::trace("add engine pointer to luau threaddata");
  lua_setthreaddata(lua, this);

  spdlog::trace("do CWorld constructor");
  world = std::make_unique<World::CWorld>();

  systems = std::make_unique<CSystemManager>(this, world.get());

  gui_manager = std::make_unique<CGuiManager>(this, world.get());

  initialized = true;
  spdlog::trace("engine ok");
}

CEngine::~CEngine() {
  spdlog::info("Destroying engine...");
  gui_manager.reset();
  if (lua)
    lua_close(lua);
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
}

void CEngine::tick(double deltaTime) { systems->tick(deltaTime); }
void CEngine::draw() { systems->draw(); }

namespace lua {
int input_is_pressed(lua_State* L) {

  Engine::CEngine* engine = static_cast<Engine::CEngine*>(lua_getthreaddata(L));
  const char* key = luaL_checkstring(L, 1); // key

  lua_pushboolean(L, engine->keyboard_state.is_down(key));

  return 1;
};
}; // namespace lua

} // namespace Engine
