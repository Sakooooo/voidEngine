#include "world.h"

void mySystem(SDL_Renderer *r) {
  auto &world{World::get_instance()};

  for (const auto &[e, transform_component] :
       world.get_component_map<Transform>()) {
    auto transform{world.get_component<Transform>(e)};
    if (!transform.has_value())
      continue;
    SDL_FRect rect{transform->get().x, transform->get().y, 500, 500};
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderFillRect(r, &rect);
  }
}
