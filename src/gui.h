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
  Gui(GuiManager *mgr) : manager(mgr) {};
  GuiManager *manager;

  bool visible = false;

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

class EntityUI : public Gui {
public:
  EntityUI(GuiManager *manager) : Gui(manager) {};
  virtual void Render();
};

class MyTestGui : public Gui {
public:
  MyTestGui(GuiManager *manager) : Gui(manager) { visible = true; };
  virtual void Render();
  EntityUI *child;
};

#endif
