#include "script.h"
#include <lauxlib.h>
#include <lua.h>

void scriptTickSystem(Engine* engine) {
  auto& world{World::get_instance()};

  for (const auto e : world.get_entities()) {
    auto query{world.view<Script>(e)};
    if (!query)
      continue;

    auto [script] = *query;

    if (!script.initalized)
      continue;

    if (script.tickRef == LUA_REFNIL)
      continue;

    lua_rawgeti(engine->lua, LUA_REGISTRYINDEX, script.tickRef);

    int result = lua_pcall(engine->lua, 0, 0, 0);

    if (result == LUA_ERRRUN) {
      const char* err = lua_tostring(engine->lua, -1);
      SDL_Log("Lua error: %s", err);
      lua_pop(engine->lua, -1);
      return;
    } else if (result == LUA_ERRMEM) {
      SDL_Log("LUA_ERRMEM");
      return;
    } else if (result == LUA_ERRERR) {
      SDL_Log("LUA_ERRERR");
      return;
    } else if (result == LUA_ERRGCMM) {
      SDL_Log("LUA_ERRGCMM");
      return;
    }
  }
}
