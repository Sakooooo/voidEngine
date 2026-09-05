#include "gui.h"
#include <algorithm>
#include <stdio.h>

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

  // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
  //             1000.0f / manager->io->Framerate, manager->io->Framerate);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / manager->io->Framerate, manager->io->Framerate);

  ImGui::End();
}
