#include "world.h"

void mySystem(SDL_Renderer *r) {
  auto &world{World::get_instance()};

  for (const auto e : world.get_entities()) {
    auto comps = world.view<Transform, Color>(e);
    if (!comps)
      continue;

    auto [transform, color] = *comps;
    SDL_FRect rect{transform.x, transform.y, 500, 500};
    SDL_SetRenderDrawColor(r, static_cast<Uint8>(color.r),
                           static_cast<Uint8>(color.g),
                           static_cast<Uint8>(color.b),
                           static_cast<Uint8>(color.a));
    SDL_RenderFillRect(r, &rect);
  }
}

void funnyRainbowSystem() {
  auto &world{World::get_instance()};
  for (const auto e : world.get_entities()) {
    auto comps = world.view<Color, Rainbow>(e);
    if (!comps)
      continue;

    auto [color, rainbow] = *comps;

    // TODO: scale by frame delta time instead of per-frame stepping
    const int step = static_cast<int>(5.0f * rainbow.speed);
    color.r = (color.r + step) % 256;
    color.g = (color.g + step) % 256;
    color.b = (color.b + step) % 256;
  }
}
