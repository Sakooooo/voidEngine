#ifndef ENGINE_H
#define ENGINE_H

#include "system.h"
#include "types.h"
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <lua.h>
#include <memory>

// forward decls
class World;
class CGuiManager;

namespace Engine {

class CEngine {
public:
  // I wonder if this could be an array later
  SDL_Window* window{nullptr};
  SDL_Renderer* renderer{nullptr};
  const bool* keyboard_state{SDL_GetKeyboardState(nullptr)};

  std::unique_ptr<World> world;
  std::unique_ptr<CSystemManager> systems;
  std::unique_ptr<CGuiManager> gui_manager;

  View2D view;

  bool initialized{false};
  lua_State* lua{nullptr};

  CEngine();
  ~CEngine();

  CEngine(const CEngine&) = delete;
  CEngine& operator=(const CEngine&) = delete;
  CEngine(CEngine&&) = delete;
  CEngine& operator=(CEngine&&) = delete;

  void tick(double deltaTime);
  void draw();
};

} // namespace Engine

#endif
