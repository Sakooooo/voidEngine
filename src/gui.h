#ifndef GUI_H
#define GUI_H
#include "imgui.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

// forward decls
class Engine;

/// The main GUI manager class, currently manages ImGui windows for now.
class CGuiManager {
public:
  CGuiManager(Engine* engine);

  ImGuiIO* io{nullptr};
  Engine* engine;

  /// Handle SDL event
  void event(SDL_Event* event);
  /// Begin ImGui
  void begin();
  /// Render every stored ImGui gui class
  void render();
  /// Finish and continue for next frame
  void end();
};

#endif
