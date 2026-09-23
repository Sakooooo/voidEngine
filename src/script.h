// Let's make games in SmileBASIC!
#ifndef SCRIPT_H
#define SCRIPT_H

#include "world.h"
#include <SDL3/SDL_log.h>
#include <filesystem>
#include <lauxlib.h>
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
    SDL_Log("OnLoad for Script was called.");
  }
};

struct SharedBool : public Component {
  bool value;
};

#endif
