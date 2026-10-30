#include "gui.h"
#include "engine.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

CGuiManager::CGuiManager(Engine* engine, World* world) {
  spdlog::trace("creating CGuiManager");
  this->engine = engine;
  this->world = world;
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

CGuiManager::~CGuiManager() {
  spdlog::info("Destroying CGuiManager");
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
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
    gui->render(engine, world);
  }

  ImGui::Render();
}

void CGuiManager::end() {
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), engine->renderer);
}

void CGuiManager::add_gui(IGui* gui) { m_guis.push_back(gui); }

void CGuiManager::remove_gui(IGui* gui) {
  std::erase(m_guis, gui);
}
