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
  int tickRef{};
  bool initalized{false};

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

    tickRef = luaL_ref(engine->lua, LUA_REGISTRYINDEX);
    if (tickRef == LUA_REFNIL) {
      SDL_Log("luaL_ref returned LUA_REFNIL!");
      SDL_Log("There is no tick() for this script!");
    }

    SDL_Log("OnLoad for Script ran successfully.");
    initalized = true;
  }
};

struct SharedBool : public Component {
  bool value;
};

void scriptTickSystem(Engine* engine);

#endif
