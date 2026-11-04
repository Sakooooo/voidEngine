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
      // letters
      {"a", SDL_SCANCODE_A},
      {"b", SDL_SCANCODE_B},
      {"c", SDL_SCANCODE_C},
      {"d", SDL_SCANCODE_D},
      {"e", SDL_SCANCODE_E},
      {"f", SDL_SCANCODE_F},
      {"g", SDL_SCANCODE_G},
      {"h", SDL_SCANCODE_H},
      {"i", SDL_SCANCODE_I},
      {"j", SDL_SCANCODE_J},
      {"k", SDL_SCANCODE_K},
      {"l", SDL_SCANCODE_L},
      {"m", SDL_SCANCODE_M},
      {"n", SDL_SCANCODE_N},
      {"o", SDL_SCANCODE_O},
      {"p", SDL_SCANCODE_P},
      {"q", SDL_SCANCODE_Q},
      {"r", SDL_SCANCODE_R},
      {"s", SDL_SCANCODE_S},
      {"t", SDL_SCANCODE_T},
      {"u", SDL_SCANCODE_U},
      {"v", SDL_SCANCODE_V},
      {"w", SDL_SCANCODE_W},
      {"x", SDL_SCANCODE_X},
      {"y", SDL_SCANCODE_Y},
      {"z", SDL_SCANCODE_Z},

      // numbers
      {"0", SDL_SCANCODE_0},
      {"1", SDL_SCANCODE_1},
      {"2", SDL_SCANCODE_2},
      {"3", SDL_SCANCODE_3},
      {"4", SDL_SCANCODE_4},
      {"5", SDL_SCANCODE_5},
      {"6", SDL_SCANCODE_6},
      {"7", SDL_SCANCODE_7},
      {"8", SDL_SCANCODE_8},
      {"9", SDL_SCANCODE_9},

      // function keys
      {"f1", SDL_SCANCODE_F1},
      {"f2", SDL_SCANCODE_F2},
      {"f3", SDL_SCANCODE_F3},
      {"f4", SDL_SCANCODE_F4},
      {"f5", SDL_SCANCODE_F5},
      {"f6", SDL_SCANCODE_F6},
      {"f7", SDL_SCANCODE_F7},
      {"f8", SDL_SCANCODE_F8},
      {"f9", SDL_SCANCODE_F9},
      {"f10", SDL_SCANCODE_F10},
      {"f11", SDL_SCANCODE_F11},
      {"f12", SDL_SCANCODE_F12},

      // arrows
      {"up", SDL_SCANCODE_UP},
      {"down", SDL_SCANCODE_DOWN},
      {"left", SDL_SCANCODE_LEFT},
      {"right", SDL_SCANCODE_RIGHT},

      // modifiers
      {"lshift", SDL_SCANCODE_LSHIFT},
      {"rshift", SDL_SCANCODE_RSHIFT},
      {"lctrl", SDL_SCANCODE_LCTRL},
      {"rctrl", SDL_SCANCODE_RCTRL},
      {"lalt", SDL_SCANCODE_LALT},
      {"ralt", SDL_SCANCODE_RALT},
      {"lgui", SDL_SCANCODE_LGUI},
      {"rgui", SDL_SCANCODE_RGUI},

      // misc
      {"space", SDL_SCANCODE_SPACE},
      {"return", SDL_SCANCODE_RETURN},
      {"escape", SDL_SCANCODE_ESCAPE},
      {"backspace", SDL_SCANCODE_BACKSPACE},
      {"tab", SDL_SCANCODE_TAB},
      {"capslock", SDL_SCANCODE_CAPSLOCK},
      {"insert", SDL_SCANCODE_INSERT},
      {"delete", SDL_SCANCODE_DELETE},
      {"home", SDL_SCANCODE_HOME},
      {"end", SDL_SCANCODE_END},
      {"pageup", SDL_SCANCODE_PAGEUP},
      {"pagedown", SDL_SCANCODE_PAGEDOWN},
      {"minus", SDL_SCANCODE_MINUS},
      {"equals", SDL_SCANCODE_EQUALS},
      {"leftbracket", SDL_SCANCODE_LEFTBRACKET},
      {"rightbracket", SDL_SCANCODE_RIGHTBRACKET},
      {"backslash", SDL_SCANCODE_BACKSLASH},
      {"semicolon", SDL_SCANCODE_SEMICOLON},
      {"apostrophe", SDL_SCANCODE_APOSTROPHE},
      {"grave", SDL_SCANCODE_GRAVE},
      {"comma", SDL_SCANCODE_COMMA},
      {"period", SDL_SCANCODE_PERIOD},
      {"slash", SDL_SCANCODE_SLASH},
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
