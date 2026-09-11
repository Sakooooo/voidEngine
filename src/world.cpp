#include "world.h"

void mySystem(SDL_Renderer *r)
{
  auto &world{World::get_instance()};

  for (const auto e : world.get_storage<Transform>().get_entities())
  {
    auto *transform{world.get_component<Transform>(e)};
    auto *color{world.get_component<Color>(e)};

    if (transform)
    {
      SDL_FRect rect{transform->x, transform->y, 500, 500};
      if (color)
        SDL_SetRenderDrawColor(r, color->r, color->g, color->b, color->a);
      else
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
      SDL_RenderFillRect(r, &rect);
    }
  }
}
