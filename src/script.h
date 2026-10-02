// Let's make games in SmileBASIC!
#ifndef SCRIPT_H
#define SCRIPT_H

#include "world.h"
#include <SDL3/SDL_log.h>
#include <cstring>
#include <filesystem>
#include <lua.h>
#include <luacode.h>
#include <lualib.h>

struct Script : public Component {
  // TODO: We're going to need something like res:// to make this a lot easier
  const char* path;
  int tickRef{};
  int meRef{}; // everything from the void table
  bool initalized{false};

  void onLoad(Engine* engine, Entity e) {

    const char* test = "print('luau is some bs', 1 + 2)";
    size_t test_size;

    char* bytecode = luau_compile(test, strlen(test), nullptr, &test_size);
    int result = luau_load(engine->lua, "chunk", bytecode, test_size, 0);
    free(bytecode);

    if (result == 0)
      lua_pcall(engine->lua, 0, 0, 0);
    else
      printf("error: %s\n", lua_tostring(engine->lua, -1));

    SDL_Log("OnLoad for Script ran successfully.");
    initalized = true;
  }
};

void scriptTickSystem(Engine* engine, double dt);

#endif
