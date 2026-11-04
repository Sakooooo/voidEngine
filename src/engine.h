#ifndef ENGINE_H
#define ENGINE_H

#include "lualib.h"
#include "system.h"
#include "types.h"
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <lua.h>
#include <map>
#include <memory>
#include <spdlog/spdlog.h>

// forward decls
namespace World {
class CWorld;
}
class CGuiManager;

namespace Engine {

struct KeyboardState {
  const bool* keys{SDL_GetKeyboardState(nullptr)};
  std::map<std::string, uint32_t> string_key_map = {
      {"w", SDL_SCANCODE_W},
  };
  KeyboardState() { spdlog::trace("create keyboard state"); };

  bool is_down(const std::string& key) const {
    auto it = string_key_map.find(key);
    if (it == string_key_map.end())
      return false;
    return keys[it->second];
  };
};

class CEngine {
public:
  // I wonder if this could be an array later
  SDL_Window* window{nullptr};
  SDL_Renderer* renderer{nullptr};
  KeyboardState keyboard_state;
  // const bool* keyboard_state{SDL_GetKeyboardState(nullptr)};

  std::unique_ptr<World::CWorld> world;
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

/// Namespace for things that get passed into luau
namespace lua {
int input_is_pressed(lua_State* L);
}; // namespace lua

} // namespace Engine

#endif
