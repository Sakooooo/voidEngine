#ifndef GUI_H
#define GUI_H
#include "engine.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
#include <vector>
class GuiManager;

class Gui {
public:
  GuiManager *manager;

  virtual void Render() = 0;
};

class GuiManager {
public:
  GuiManager(SDL_Window *window, SDL_Renderer *renderer);
  ~GuiManager();

  ImGuiIO *io;
  bool initalized = false;

  void AddPanel(Gui *panel);
  void RemovePanel(Gui *panel);

  void RenderPanels();

private:
  std::vector<Gui *> panels;
};

class MyTestGui : public Gui {
public:
  MyTestGui(GuiManager *manager) { manager = manager; };
  virtual void Render();
};

#endif
