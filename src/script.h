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
  int meRef{}; // everything from the void table
  bool initalized{false};

  void onLoad(Engine* engine, Entity e) {
    SDL_Log("OnLoad for Script ran successfully.");
    initalized = true;
  }
};

void scriptTickSystem(Engine* engine, double dt);

#endif
