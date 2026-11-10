#include "world.h"
#include <lua.h>
#include <lualib.h>
#include <spdlog/spdlog.h>

int World::lua::has(lua_State* L) {
  Engine::CEngine* engine = static_cast<Engine::CEngine*>(lua_getthreaddata(L));

  luaL_checktype(L, 1, LUA_TTABLE);                                    // self
  lua_getfield(L, 1, "id");                                            // self
  const Entity entity = static_cast<Entity>(luaL_checkinteger(L, -1)); // grab
  lua_pop(L, 1);

  const char* name = luaL_checkstring(L, 2); // type
  spdlog::trace("Checking {}", name);
  lua_pushboolean(L, engine->world->has_component_by_name(name, entity));

  return 1;
}

int World::lua::transform2d_lua_index(lua_State* L) {
  Engine::CEngine* engine = static_cast<Engine::CEngine*>(lua_getthreaddata(L));
  EntityReference ref =
      *static_cast<EntityReference*>(luaL_checkudata(L, 1, "Transform2D"));
  std::string key = static_cast<std::string>(luaL_checkstring(L, 2));

  if (!engine->world->is_valid(ref) ||
      !engine->world->has_component<Transform2D>(ref.entity)) {
    spdlog::warn("Transform2D expired for {}", ref.entity);
    luaL_error(L, "Component Transform2D for this generation has expired.");
  };

  Transform2D& transform =
      engine->world->get_component<Transform2D>(ref.entity);

  if (key == "width") {
    lua_pushnumber(L, transform.width);
  } else if (key == "height") {
    lua_pushnumber(L, transform.height);
  } else if (key == "pos") {
    Vector2* pos = static_cast<Vector2*>(lua_newuserdata(L, sizeof(Vector2)));
    *pos = transform.pos;
    luaL_getmetatable(L, "Vector2");
    lua_setmetatable(L, -2);
  } else {
    lua_pushnil(L);
  };
  return 1;
}

int World::lua::entity_lua_index(lua_State* L) {
  Engine::CEngine* engine = static_cast<Engine::CEngine*>(lua_getthreaddata(L));
  std::string key = static_cast<std::string>(luaL_checkstring(L, 2));

  lua_pushstring(L, "id");
  lua_rawget(L, 1); // no metamethods
  Entity entity = static_cast<Entity>(lua_tointeger(L, -1));
  lua_pop(L, 1);

  if (key == "Transform2D" &&
      engine->world->has_component<Transform2D>(entity)) {

    // because on component removal, freed memory would get accessed again
    EntityReference* ref = static_cast<EntityReference*>(
        lua_newuserdata(L, sizeof(EntityReference)));
    *ref = engine->world->make_ref(entity);

    luaL_getmetatable(L, "Transform2D");
    lua_setmetatable(L, -2);
  } else {
    lua_pushnil(L);
  }

  return 1;
}

int World::lua::transform2d_lua_newindex(lua_State* L) {
  Engine::CEngine* engine = static_cast<Engine::CEngine*>(lua_getthreaddata(L));
  EntityReference ref =
      *static_cast<EntityReference*>(luaL_checkudata(L, 1, "Transform2D"));
  std::string key = static_cast<std::string>(luaL_checkstring(L, 2));

  if (!engine->world->is_valid(ref) ||
      !engine->world->has_component<Transform2D>(ref.entity)) {
    spdlog::trace("Transform2D expired for {}", ref.entity);
    luaL_error(L, "Transform2D has expired");
  }

  Transform2D& transform =
      engine->world->get_component<Transform2D>(ref.entity);

  if (key == "pos") {
    transform.pos = *static_cast<Vector2*>(luaL_checkudata(L, 3, "Vector2"));
  } else if (key == "width") {
    transform.width = static_cast<float>(luaL_checknumber(L, 3));
  } else if (key == "height") {
    transform.height = static_cast<float>(luaL_checknumber(L, 3));
  } else {
    luaL_error(L, "%s is not a valid field on Transform2D", key.c_str());
  }

  return 0;
}
