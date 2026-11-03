// TODO: hot reloading
#include "script.h"
#include "lua.h"
#include "luacode.h"
#include "lualib.h"
#include "world.h"
#include <SDL3/SDL_filesystem.h>
#include <spdlog/spdlog.h>

bool CLuauScriptSystem::load_script(lua_State* L, Entity entity,
                                    const std::string& path,
                                    int& environment_ref) {

  const char* base_path = SDL_GetBasePath();
  if (!base_path) {
    spdlog::error("Failed to get base path {}", SDL_GetError());
    return false;
  }

  std::string full_path = std::string(base_path) + "scripts/" + path;
  spdlog::trace("Loading Luau script from {}", full_path);

  size_t file_length = 0;
  char* src = static_cast<char*>(SDL_LoadFile(full_path.c_str(), &file_length));
  if (!src) {
    spdlog::error("failed to load script at {}, {}", full_path, SDL_GetError());
    return false;
  }

  spdlog::trace("compiling {} to bytecode", full_path);
  size_t bytecode_length = 0;
  lua_CompileOptions options{};
  options.optimizationLevel = 1;
  options.debugLevel = 1;
  char* bytecode = luau_compile(src, file_length, &options, &bytecode_length);
  SDL_free(src);
  if (!bytecode) {
    spdlog::error("failed to allocate bytecode for {}", full_path);
    return false;
  }
  if (bytecode_length > 0 && bytecode[0] == 0) {
    spdlog::error("failed to compile {}{}", path,
                  std::string_view(bytecode + 1, bytecode_length - 1));
    free(bytecode);
    return false;
  }
  spdlog::trace("compiled {} to bytecode", full_path);

  lua_newtable(L);                    // environment
  lua_newtable(L);                    // void table
  lua_setfield(L, -2, "void");        // set name of table
  lua_pushinteger(L, entity);         // push entity id
  lua_setfield(L, -2, "entity");      // set entity id on environment table
  lua_newtable(L);                    // metatable __index
  lua_pushvalue(L, LUA_GLOBALSINDEX); // push globals
  lua_setfield(L, -2, "__index");     // set __index of metatable
  lua_setmetatable(L, -2);            // make it the actual metatable

  std::string chunk = "=" + path;
  int status = luau_load(L, chunk.c_str(), bytecode, bytecode_length, -1);
  free(bytecode); // no need for this anymore
  if (status != 0 || lua_pcall(L, 0, 0, 0) != 0) {
    spdlog::error("luau error {}: {}", full_path,
                  luaL_tolstring(L, -1, nullptr));
    lua_pop(L, 3);
    return false;
  }
  environment_ref = lua_ref(L, -1); // Store the environment in a reference
  lua_pop(L, 1);                    // cleanup
  spdlog::trace("Successfully loaded luau script {}", full_path);
  return true;
};

void CLuauScriptSystem::tick(Engine::CEngine* engine, World::CWorld* world,
                             double deltaTime) {
  lua_State* L = engine->lua;
  world->each<LuauScript>([&](Entity entity, LuauScript& script) {
    auto it = m_instances.find(entity);
    if (it != m_instances.end() &&
        (!world->is_valid(it->second.ref) || it->second.path != script.path ||
         script.reload)) {
      // discard
      lua_unref(L, it->second.environment_ref);
      m_instances.erase(it);
      it = m_instances.end();
    }
    script.reload = false;
    if (it == m_instances.end()) {
      CLuauScriptSystem::ScriptInstance instance{};
      instance.ref = world->make_ref(entity);
      instance.path = script.path;
      instance.failed =
          !load_script(L, entity, script.path, instance.environment_ref);
      it = m_instances.emplace(entity, instance).first;
    }
    if (it->second.failed)
      return;

    lua_getref(L, it->second.environment_ref);
    // TODO: move this into load_script to cache this instead
    lua_getfield(L, -1, "void"); // grab void table
    if (!lua_istable(L, -1)) {
      spdlog::error("lua {}: void isn't a table!", script.path);
      it->second.failed = true;
      lua_pop(L, 2);
      return;
    }
    lua_getfield(L, -1, "tick"); // get tick function from table
    if (lua_isfunction(L, -1)) {
      lua_pushinteger(L, entity);
      lua_pushnumber(L, deltaTime);
      if (lua_pcall(L, 2, 0, 0) != 0) {

        spdlog::error("lua error {}: {}", script.path,
                      luaL_tolstring(L, -1, nullptr));
        lua_pop(L, 2);
        it->second.failed = true;
      }
    } else {
      lua_pop(L, 1);
    }
    lua_pop(L, 2);
  });

  std::erase_if(m_instances, [&](auto& instance) {
    if (world->is_valid(instance.second.ref) &&
        world->has_component<LuauScript>(instance.first))
      return false;
    lua_unref(L, instance.second.environment_ref);
    return true;
  });
}
