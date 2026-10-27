#include "gui.h"
#include "engine.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

CGuiManager::CGuiManager(Engine* engine) {
  spdlog::trace("creating CGuiManager");
  this->engine = engine;
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO();

  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGuiStyle& style = ImGui::GetStyle();
  ImGui::StyleColorsDark();
  style.FrameRounding = 3.0f;
  style.GrabRounding = 3.0f;

  spdlog::trace("CGuiManager sdl3 impl init");
  ImGui_ImplSDL3_InitForSDLRenderer(engine->window, engine->renderer);
  ImGui_ImplSDLRenderer3_Init(engine->renderer);
}

void CGuiManager::event(SDL_Event* event) {
  ImGui_ImplSDL3_ProcessEvent(event);
}

void CGuiManager::begin() {
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

void CGuiManager::render() {
  // iterate through every stored gui in m_guis
  for (auto& gui : m_guis) {
    // check if it's been freed
    if (!gui) {
      continue;
    }

    // check if name has been set
    if (!gui->name) {
      ImGui::Begin(gui->name);
    } else {
      ImGui::Begin("CGuiManager_default");
    }

    gui->render();

    ImGui::End();
  }

  ImGui::Render();
}

void CGuiManager::end() {
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), engine->renderer);
}

void CGuiManager::add_gui(IGui* gui) { m_guis.push_back(gui); }

void CGuiManager::remove_gui(IGui* gui) {
  // Search through the entire vector for the system to remove
  m_guis.erase(std::remove(m_guis.begin(), m_guis.end(), gui));
}
