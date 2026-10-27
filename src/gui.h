#ifndef GUI_H
#define GUI_H
#include "imgui.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <spdlog/spdlog.h>
#include <vector>

// forward decls
class Engine;

/// The interface for any GUIs, first derive a new class from IGui, override
/// it's render() function with what's need then add it to CGuiManager with
/// add_gui()
class IGui {
public:
  IGui(const char* name = nullptr) {
    this->name = name;
    spdlog::trace("Creaitng a new Gui class");
  };

  /// Title name
  const char* name;

  /// Render the ImGui GUI
  virtual void render() = 0;
};

/// The main GUI manager class, currently manages ImGui windows for now.
class CGuiManager {
public:
  CGuiManager(Engine* engine);

  ImGuiIO* io{nullptr};
  Engine* engine;

  /// Add derived IGui class to m_guis
  void add_gui(IGui* gui);

  /// Remove a derived IGui class from m_guis
  void remove_gui(IGui* gui);

  /// Handle SDL event
  void event(SDL_Event* event);
  /// Begin ImGui
  void begin();
  /// Render every stored ImGui gui class
  void render();
  /// Finish and continue for next frame
  void end();

private:
  std::vector<IGui*> m_guis;
};

#endif
