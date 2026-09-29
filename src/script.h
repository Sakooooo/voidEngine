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
    luaL_dostring(engine->lua, "print('test')");
    if (!path) {
      SDL_Log("Path was nullptr");
      return;
    }

    if (!std::filesystem::exists(path)) {
      SDL_Log("Could not find script at %s", path);
      return;
    }

    if (luaL_loadfile(engine->lua, path) != LUA_OK) {
      SDL_Log("Failed to load Lua file! %s", lua_tostring(engine->lua, -1));
      lua_pop(engine->lua, 1);
      return;
    }; // script

    // script environment setup
    lua_newtable(engine->lua); // environment
    lua_newtable(engine->lua); // metatable
    lua_pushglobaltable(engine->lua);
    lua_setfield(engine->lua, -2,
                 "__index"); // metatable functions equal to global functions
    lua_setmetatable(engine->lua, -2); // set metatable

    // me table
    lua_newtable(engine->lua); // push me table
    lua_pushvalue(engine->lua, -1);
    meRef = luaL_ref(engine->lua,
                     LUA_REGISTRYINDEX); // grab reference for later use
    lua_setfield(engine->lua, -2,
                 "me"); // set the environment table to have our new me table
    lua_setupvalue(
        engine->lua, -2,
        1); // set the script's environment table to our environment table

    if (lua_pcall(engine->lua, 0, 0, 0) != LUA_OK) {
      SDL_Log("Error running script %s, %s:", path,
              lua_tostring(engine->lua, -1));
      lua_pop(engine->lua, 2);
      return;
    }

    lua_rawgeti(engine->lua, LUA_REGISTRYINDEX, meRef);

    lua_getfield(engine->lua, -1, "tick");
    if (!lua_isfunction(engine->lua, -1)) {
      SDL_Log("Couldn't find function for tick() or it's not even a function!");
      lua_pop(engine->lua, 1);
      return;
    }
    tickRef = luaL_ref(engine->lua, LUA_REGISTRYINDEX);
    if (tickRef == LUA_REFNIL) {
      SDL_Log("luaL_ref returned LUA_REFNIL for tick()!");
    }
    lua_pop(engine->lua, 1);

    SDL_Log("OnLoad for Script ran successfully.");
    initalized = true;
  }
};

struct SharedBool : public Component {
  bool value;
};

void scriptTickSystem(Engine* engine, double dt);

#endif
