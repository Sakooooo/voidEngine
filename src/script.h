// Let's make games in SmileBASIC!
#ifndef SCRIPT_H
#define SCRIPT_H

#include "world.h"
#include <SDL3/SDL_log.h>
#include <filesystem>
#include <lauxlib.h>
#include <lua.h>
#include <lua.hpp>

struct Script : public Component {
  // TODO: We're going to need something like res:// to make this a lot easier
  const char* path;

  void onLoad(Engine* engine, Entity e) {
    luaL_dostring(engine->lua, "print('test')");
    if (!path) {
      SDL_Log("Path was nullptr");
      return;
    }

    if (!std::filesystem::exists(path)) {
      SDL_Log("Could not find script at %s", path);
      return;
    }

    luaL_dofile(engine->lua, path);

    lua_getglobal(engine->lua, "tick");

    if (!lua_isfunction(engine->lua, -1)) {
      SDL_Log("Couldn't find function for tick() or it's not even a function!");
      return;
    }

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

    SDL_Log("OnLoad for Script ran successfully.");
  }
};

struct SharedBool : public Component {
  bool value;
};

#endif
