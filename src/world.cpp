#include "world.h"

void mySystem(SDL_Renderer *r) {
  auto &world{World::get_instance()};

  for (const auto e : world.get_storage<Transform>().get_entities()) {
    auto *transform{world.get_component<Transform>(e)};
    auto *color{world.get_component<Color>(e)};
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
