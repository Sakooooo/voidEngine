#include "world.h"
#include "imgui.h"

void mySystem(SDL_Renderer *r) {
  auto &world{World::get_instance()};

  for (const auto e : world.get_entities()) {
    auto comps = world.view<Transform, Color>(e);
    if (!comps)
      continue;

    auto [transform, color] = *comps;
    SDL_FRect rect{transform.x, transform.y, 500, 500};
    SDL_SetRenderDrawColor(
        r, static_cast<Uint8>(color.r), static_cast<Uint8>(color.g),
        static_cast<Uint8>(color.b), static_cast<Uint8>(color.a));
    SDL_RenderFillRect(r, &rect);
  }
}

void funnyRainbowSystem(double dt) {
  auto &world{World::get_instance()};
  for (const auto e : world.get_entities()) {
    auto comps = world.view<Color, Rainbow>(e);
    if (!comps)
      continue;

    auto [color, rainbow] = *comps;

    rainbow.hue = std::fmod(rainbow.hue + rainbow.speed * dt, 1.0f);

    float r, g, b;
    ImGui::ColorConvertHSVtoRGB(rainbow.hue, 1.0f, 1.0f, r, g, b);
    color.r = static_cast<int>(r * 255);
    color.g = static_cast<int>(g * 255);
    color.b = static_cast<int>(b * 255);
  }
}
