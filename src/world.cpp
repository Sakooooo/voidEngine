#include "world.h"
#include <lua.h>
#include <lualib.h>
#include <spdlog/spdlog.h>

int World::lua::has(lua_State* L) {
  Engine::CEngine* engine = static_cast<Engine::CEngine*>(lua_getthreaddata(L));

  luaL_checktype(L, 1, LUA_TTABLE); // self
  lua_getfield(L, 1, "id"); // self
  const Entity entity = static_cast<Entity>(luaL_checkinteger(L, -1)); // grab
  lua_pop(L, 1);

  const char* name = luaL_checkstring(L, 2); // type
  spdlog::trace("Checking {}", name);
  lua_pushboolean(L, engine->world->has_component_by_name(name, entity));

  return 1;
}
