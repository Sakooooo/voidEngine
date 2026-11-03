#include "engine.h"
#include "guis.h"
#include "imgui.h"
#include "world.h"
#include <SDL3/SDL_render.h>
#include <algorithm>
#include <lua.h>
#include <spdlog/spdlog.h>

void DebugGui::render(Engine::CEngine* engine, World* world) {
  ImGuiIO& io = ImGui::GetIO();

  m_frame_times[m_frame_offset] = io.DeltaTime * 1000.0f;
  m_frame_offset = (m_frame_offset + 1) % m_frame_times.size();

  ImGui::Begin("Debug");

  if (ImGui::CollapsingHeader("Performance", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Text("%.1f FPS (%.3f ms)", io.Framerate, 1000.0f / io.Framerate);
    float max_time =
        *std::max_element(m_frame_times.begin(), m_frame_times.end());
    ImGui::PlotLines("##frame_times", m_frame_times.data(),
                     static_cast<int>(m_frame_times.size()),
                     static_cast<int>(m_frame_offset), "frame time (ms)", 0.0f,
                     std::max(max_time, 1.0f), ImVec2(-FLT_MIN, 60.0f));

    int vsync = 0;
    SDL_GetRenderVSync(engine->renderer, &vsync);
    bool vsync_enabled = vsync != 0;
    if (ImGui::Checkbox("VSync", &vsync_enabled))
      SDL_SetRenderVSync(engine->renderer, vsync_enabled ? 1 : 0);
  }

  if (ImGui::CollapsingHeader("World", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Text("Entities: %zu", world->all_entities().size());
    // the camera system overwrites the view every frame, so it's display only
    ImGui::Text("View center: %.1f, %.1f", engine->view.center.x,
                engine->view.center.y);
    ImGui::Text("View zoom: %.2f", engine->view.zoom);
  }

  if (ImGui::CollapsingHeader("Luau")) {
    ImGui::Text("Memory: %d KB", lua_gc(engine->lua, LUA_GCCOUNT, 0));
    if (ImGui::Button("Collect Garbage"))
      lua_gc(engine->lua, LUA_GCCOLLECT, 0);
  }

  if (ImGui::CollapsingHeader("Logging")) {
    static const char* levels[] = {"trace", "debug",    "info", "warn",
                                   "error", "critical", "off"};
    static const spdlog::level::level_enum level_values[] = {
        spdlog::level::trace, spdlog::level::debug, spdlog::level::info,
        spdlog::level::warn,  spdlog::level::err,   spdlog::level::critical,
        spdlog::level::off};
    auto current = spdlog::get_level();
    int selected = 0;
    for (int i = 0; i < IM_ARRAYSIZE(level_values); ++i) {
      if (level_values[i] == current)
        selected = i;
    }
    if (ImGui::Combo("Log level", &selected, levels, IM_ARRAYSIZE(levels)))
      spdlog::set_level(level_values[selected]);
  }

  if (ImGui::CollapsingHeader("ImGui")) {
    ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
    ImGui::Checkbox("Show demo window", &m_show_demo);
    ImGui::Checkbox("Show metrics window", &m_show_metrics);
  }

  ImGui::End();

  if (m_show_demo)
    ImGui::ShowDemoWindow(&m_show_demo);
  if (m_show_metrics)
    ImGui::ShowMetricsWindow(&m_show_metrics);
}
