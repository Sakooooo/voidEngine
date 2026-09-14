#include "world.h"
#include <cmath>

void mySystem(SDL_Renderer *r) {
  auto &world{World::get_instance()};

  for (const auto e : world.get_entities()) {
    // auto *transform{world.get_component<Transform>(e)};
    // auto *color{world.get_component<Color>(e)};
    auto entityComps = world.view<Transform, Color>(e);

    if (auto comps = entityComps) {
      {
        auto [transform, color] = *comps;
        SDL_FRect rect{transform.x, transform.y, 500, 500};
        SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(r, &rect);
      }
    }
  }
}

void funnyRainbowSystem() {
  auto &world{World::get_instance()};
  for (const auto e : world.get_entities()) {

    auto entityComps = world.view<Color, Rainbow>(e);

    if (auto comps = entityComps) {
      auto [color, rainbow] = *comps;

      color.r = color.r >= 255 ? 0 : color.r + 5;
      color.g = color.g >= 255 ? 0 : color.g + 5;
      color.b = color.b >= 255 ? 0 : color.b + 5;
    }
  }
}
