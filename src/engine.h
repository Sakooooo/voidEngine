#ifndef ENGINE_H
#define ENGINE_H

#include "system.h"
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <lua.h>
#include <memory>

class World;

class Engine {
public:
  // I wonder if this could be an array later
  SDL_Window* window{nullptr};
  SDL_Renderer* renderer{nullptr};
  const bool* keyboard_state{SDL_GetKeyboardState(nullptr)};

  std::unique_ptr<World> world;
  std::unique_ptr<CSystemManager> systems;

  bool initialized{false};
  lua_State* lua{nullptr};

  Engine();
  ~Engine();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  Engine(Engine&&) = delete;
  Engine& operator=(Engine&&) = delete;

  void tick(double deltaTime);
  void draw();
};

#endif
