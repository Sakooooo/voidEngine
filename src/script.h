// Let's make games in SmileBASIC!
#ifndef SCRIPT_H
#define SCRIPT_H

#include "world.h"
#include <lua.hpp>
#include <SDL3/SDL_log.h>

struct Script : public Component {
  const char* filepath; // TODO replace with std::string or std::filesystem::path

  void onLoad(Engine* engine, Entity e) {
    luaL_dostring(engine->lua, "print('test')");
    SDL_Log("OnLoad for Script was called.");
  }
};

struct SharedBool : public Component {
  bool value;
};

#endif
