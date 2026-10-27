#ifndef GUI_H
#define GUI_H
#include "imgui.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

class Engine;

class CGuiManager {
public:
  CGuiManager(Engine* engine);

  ImGuiIO* io{nullptr};
  Engine* engine;

  void event(SDL_Event* event);
  void begin();
  void render();
  void end();
};

#endif
