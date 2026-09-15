#include "gui.h"
#include "mind.h"
#include "world.h"
#include <SDL3/SDL_log.h>
#include <functional>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <string>
#include <vector>

GuiManager::GuiManager(SDL_Window *window, SDL_Renderer *renderer) {
  SDL_Log("Initializing GuiManager");
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO();
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  ImGuiStyle &style = ImGui::GetStyle();
  style.ScaleAllSizes(main_scale);
#if IMGUI_VERSION_NUM > 19200
  style.FontScaleDpi = main_scale;
#endif

  if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
    SDL_Log("ImGui_ImplSDL3_InitForSDLRenderer failed");
    return;
  }
  if (!ImGui_ImplSDLRenderer3_Init(renderer)) {
    SDL_Log("ImGui_ImplSDLRenderer3_Init failed");
    ImGui_ImplSDL3_Shutdown();
    return;
  }
  initialized = true;
}

GuiManager::~GuiManager() {
  SDL_Log("Destroying GuiManager...");
  if (initialized) {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
  }
  ImGui::DestroyContext();
}

void GuiManager::AddPanel(Gui *panel) { panels.push_back(panel); }

void GuiManager::RemovePanel(Gui *panel) { std::erase(panels, panel); }

void GuiManager::RenderPanels() {
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  for (auto *panel : panels) {
    if (panel->visible)
      panel->Render();
  }

  ImGui::Render();
}

void MyTestGui::Render() {
  static float f = 0.0f;
  static int counter = 0;

  ImGui::Begin("Hello world!");

  ImGui::Text("I am some text");

  ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

  if (ImGui::Button("Button"))
    counter++;

  ImGui::SameLine();
  ImGui::Text("counter = %d", counter);

  if (child)
    ImGui::Checkbox("Show Entity UI", &child->visible);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / manager->io->Framerate, manager->io->Framerate);

  ImGui::End();
}

void EntityUI::Render() {
  World &world{World::get_instance()};
  ImGui::Begin("Entity Viewer");

  // Removing components or destroying entities mutates the storages / entity
  // list we're iterating and holding pointers into, so queue those up and run
  // them once the loop is done.
  std::vector<std::function<void()>> deferred{};

  // TODO: This might be better as a list with popup windows?
  if (ImGui::BeginTabBar("entities")) {

    if (ImGui::BeginTabItem("Control")) {
      ImGui::Text("Number of alive entities %zu", world.get_entities().size());

      if (ImGui::Button("Create Entity")) {
        world.createEntity();
      }

      ImGui::EndTabItem();
    }

    // TODO: Make a way to get this to fetch all components of one entity
    for (const auto e : world.get_entities()) {
      auto *transform{world.get_component<Transform>(e)};
      auto *color{world.get_component<Color>(e)};
      auto *rainbow{world.get_component<Rainbow>(e)};
      auto *controllable{world.get_component<Controllable>(e)};

      std::string label = "Entity " + std::to_string(e);
      ImGui::PushID(static_cast<int>(e));
      if (ImGui::BeginTabItem(label.c_str())) {
        ImGui::Text("Transform");
        if (transform) {
          ImGui::SliderFloat("x", &transform->x, 0.0f, 2000.0f);
          ImGui::SliderFloat("y", &transform->y, 0.0f, 1000.0f);

          if (ImGui::Button("Remove Transform"))
            deferred.push_back(
                [&world, e] { world.remove_component<Transform>(e); });
        } else {
          if (ImGui::Button("Add Transform"))
            world.add_component<Transform>(e, 100.0f, 100.f);
        }

        ImGui::Text("Color");
        if (color) {
          ImGui::SliderInt("r", &color->r, 0, 255);
          ImGui::SliderInt("g", &color->g, 0, 255);
          ImGui::SliderInt("b", &color->b, 0, 255);
          ImGui::SliderInt("a", &color->a, 0, 255);
          if (ImGui::Button("Remove Color"))
            deferred.push_back(
                [&world, e] { world.remove_component<Color>(e); });
        } else {
          if (ImGui::Button("Add Color"))
            world.add_component<Color>(e, 0, 0, 0, 0);
        }

        ImGui::Text("Rainbow");
        if (rainbow) {
          ImGui::SliderFloat("speed", &rainbow->speed, 0.1f, 5.0f);
          if (ImGui::Button("Remove Rainbow"))
            deferred.push_back(
                [&world, e] { world.remove_component<Rainbow>(e); });
        } else {
          if (ImGui::Button("Add Rainbow"))
            world.add_component<Rainbow>(e, 1.0f);
        }

        ImGui::Text("Controllable");
        if (controllable) {
          if (controllable->mind.controlled) {
            if (ImGui::Button("Stop controlling"))
              controllable->mind.controlled = false;
          } else {
            if (ImGui::Button("Start controlling"))
              controllable->mind.controlled = true;
          }

          if (ImGui::Button("Remove control"))
            deferred.push_back(
                [&world, e] { world.remove_component<Controllable>(e); });
        } else {
          if (ImGui::Button("Add control"))
            world.add_component<Controllable>(e);
        }

        ImGui::Text("Control");
        if (ImGui::Button("Destroy Entity"))
          deferred.push_back([&world, e] { world.destroyEntity(e); });

        ImGui::EndTabItem();
      }
      ImGui::PopID();
    }
    ImGui::EndTabBar();
  }

  for (auto &op : deferred)
    op();

  ImGui::End();
}

void MyDebugUi::Render() {
  ImGui::Begin("Debug");

  if (entity_debug)
    ImGui::Checkbox("Show Entity Debug UI", &entity_debug->visible);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / manager->io->Framerate, manager->io->Framerate);

  ImGui::End();
}
