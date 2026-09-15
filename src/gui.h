#ifndef GUI_H
#define GUI_H
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <imgui.h>
#include <vector>

class GuiManager;

class Gui {
public:
  explicit Gui(GuiManager *mgr) : manager(mgr) {}
  virtual ~Gui() = default;

  GuiManager *manager;

  bool visible = false;

  virtual void Render() = 0;
};

class GuiManager {
public:
  GuiManager(SDL_Window *window, SDL_Renderer *renderer);
  ~GuiManager();

  // Owns the global ImGui context; copying would shut it down twice.
  GuiManager(const GuiManager &) = delete;
  GuiManager &operator=(const GuiManager &) = delete;
  GuiManager(GuiManager &&) = delete;
  GuiManager &operator=(GuiManager &&) = delete;

  ImGuiIO *io{nullptr};
  bool initialized = false;

  void AddPanel(Gui *panel);
  void RemovePanel(Gui *panel);

  void RenderPanels();

private:
  std::vector<Gui *> panels;
};

class EntityUI : public Gui {
public:
  explicit EntityUI(GuiManager *manager) : Gui(manager) {}
  void Render() override;
};

class MyTestGui : public Gui {
public:
  explicit MyTestGui(GuiManager *manager) : Gui(manager) { visible = true; }
  void Render() override;
  EntityUI *child = nullptr;
};

class MyDebugUi : public Gui {
public:
  explicit MyDebugUi(GuiManager *manager) : Gui(manager) {}
  void Render() override;
  EntityUI *entity_debug = nullptr;
};

#endif
