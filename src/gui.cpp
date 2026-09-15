#include "gui.h"
#include "mind.h"
#include "world.h"
#include <algorithm>
#include <imgui.h>
#include <stdio.h>
#include <string>
#include <utility>

GuiManager::GuiManager(SDL_Window *window, SDL_Renderer *renderer) {
  printf("Initalizing GuiManager\n");
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO();
  (void)io;
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  ImGuiStyle &style = ImGui::GetStyle();
  style.ScaleAllSizes(main_scale);
#if IMGUI_VERSION_NUM > 19200
  style.FontScaleDpi = main_scale;
#endif

  if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer) ||
      !ImGui_ImplSDLRenderer3_Init(renderer))
    initalized = false;
  else
    initalized = true;
}

GuiManager::~GuiManager() {
  printf("Destroying GuiManager...\n");
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}

void GuiManager::AddPanel(Gui *panel) { panels.push_back(panel); }

void GuiManager::RemovePanel(Gui *panel) {
  panels.erase(std::find(panels.begin(), panels.end(), panel));
}

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
  // ImGui::Checkbox("Open demo window", &show_demo_window);
  // ImGui::Checkbox("Open Another window", &show_another_window);

  ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
  // ImGui::ColorEdit3("clear color", (float *)&clear_color);

  if (ImGui::Button("Button"))
    counter++;

  ImGui::SameLine();
  ImGui::Text("counter = %d", counter);

  if (child)
    ImGui::Checkbox("Show Entity UI", &child->visible);

  // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
  //             1000.0f / manager->io->Framerate, manager->io->Framerate);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / manager->io->Framerate, manager->io->Framerate);

  ImGui::End();
}

void EntityUI::Render() {
  World &world{World::get_instance()};
  ImGui::Begin("Entity Viewer");

  std::optional<std::pair<std::type_index, Entity>> pending_removal{};

  // TODO: This might be better as a list with popup windows?
  if (ImGui::BeginTabBar("entities")) {

    ImGui::PushID(0);
    if (ImGui::BeginTabItem("Control")) {

      ImGui::Text("Number of alive entities %zu", world.get_entities().size());

      if (ImGui::Button("Create Entity")) {
        world.createEntity();
      }

      ImGui::EndTabItem();
    };
    ImGui::PopID();

    // TODO: Make a way to get this to fetch all components of one entity
    size_t i = 1; // start at 1 because Control is 0

    for (const auto e : world.get_entities()) {
      auto *transform{world.get_component<Transform>(e)};
      auto *color{world.get_component<Color>(e)};
      auto *rainbow{world.get_component<Rainbow>(e)};
      auto *controllable{world.get_component<Controllable>(e)};

      std::string label = "Entity " + std::to_string(e);
      ImGui::PushID(i);
      if (ImGui::BeginTabItem(label.c_str())) {
        ImGui::Text("Transform");
        if (transform) {
          ImGui::SliderFloat("x", &transform->x, 0.0f, 2000.0f);
          ImGui::SliderFloat("y", &transform->y, 0.0f, 1000.0f);

          if (ImGui::Button("Remove Transform")) {
            pending_removal.emplace(std::type_index(typeid(Transform)), e);
          }
        } else {
          if (ImGui::Button("Add Transform")) {
            world.add_component<Transform>(e, 100.0f, 100.f);
          }
        }

        ImGui::Text("Color");
        if (color) {
          ImGui::SliderInt("r", &color->r, 0, 255);
          ImGui::SliderInt("g", &color->g, 0, 255);
          ImGui::SliderInt("b", &color->b, 0, 255);
          ImGui::SliderInt("a", &color->a, 0, 255);
          if (ImGui::Button("Remove Color")) {
            pending_removal.emplace(std::type_index(typeid(Color)), e);
          }
        } else {
          if (ImGui::Button("Add Color")) {
            world.add_component<Color>(e, 0, 0, 0, 0);
          }
        }

        ImGui::Text("Rainbow");
        if (rainbow) {
          ImGui::SliderFloat("speed", &rainbow->speed, 0.1, 5);
          if (ImGui::Button("Remove Rainbow")) {
            pending_removal.emplace(std::type_index(typeid(Rainbow)), e);
          }
        } else {
          if (ImGui::Button("Add Rainbow")) {
            world.add_component<Rainbow>(e, 1.0f);
          }
        }

        ImGui::Text("Controllable");
        if (controllable) {
          if (controllable->mind->controlled) {
            if (ImGui::Button("Stop controlling")) {
              controllable->mind->controlled = false;
            }
          } else {
            if (ImGui::Button("Start controlling")) {
              controllable->mind->controlled = true;
            }
          }

          if (ImGui::Button("Remove control")) {
            pending_removal.emplace(std::type_index(typeid(Controllable)), e);
          }
        } else {
          if (ImGui::Button("Add control")) {
            world.add_component<Controllable>(e, new Mind());
          }
        }

        ImGui::Text("Control");
        if (ImGui::Button("Destroy Entity")) {
          world.destroyEntity(e);
        }

        ImGui::EndTabItem();
      }
      ImGui::PopID();
      i++;
    }
    ImGui::EndTabBar();

    if (pending_removal.has_value()) {
      // I'm pretty sure this is really bad
      // It's me again it's really bad holy fucking shit
      if (pending_removal->first == std::type_index(typeid(Transform)))
        world.get_storage<Transform>().remove_component(
            pending_removal->second);

      if (pending_removal->first == std::type_index(typeid(Color)))
        world.get_storage<Color>().remove_component(pending_removal->second);

      if (pending_removal->first == std::type_index(typeid(Rainbow)))
        world.get_storage<Rainbow>().remove_component(pending_removal->second);

      if (pending_removal->first == std::type_index(typeid(Controllable)))
        world.get_storage<Controllable>().remove_component(
            pending_removal->second);

      printf("If it's not working it's because you probably forgot to add the "
             "component to gui.cpp\n");
    }
  }

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