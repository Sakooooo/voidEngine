#ifndef ENGINE_H
#define ENGINE_H

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <memory>

// gui.h doesn't need anything from engine.h, so a forward declaration is all
// that's needed here. Including gui.h from this header (while gui.h included
// engine.h) created an include cycle, which is why clang "couldn't find" it.
class GuiManager;

class Engine {
public:
  // I wonder if this could be an array later
  SDL_Window *window{nullptr};
  SDL_Renderer *renderer{nullptr};
  const bool *keyboard_state{SDL_GetKeyboardState(nullptr)};

  std::unique_ptr<GuiManager> gui_manager;

  bool initialized{false};

  Engine();
  ~Engine();

  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;
  Engine(Engine &&) = delete;
  Engine &operator=(Engine &&) = delete;
};

#endif
