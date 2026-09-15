// IT'S GOT A MIND OF IT'S OWN OH GOD
#include "mind.h"

void mindSystem(const SDL_Event &event) {
  if (event.type != SDL_EVENT_KEY_DOWN)
    return;

  auto &world{World::get_instance()};

  for (const auto e : world.get_entities()) {
    auto query{world.view<Controllable, Transform>(e)};
    if (!query)
      continue;

    auto [controllable, transform] = *query;
    if (!controllable.mind.controlled)
      continue;

    switch (event.key.key) {
    case SDLK_W:
      transform.y -= 5;
      break;
    case SDLK_A:
      transform.x -= 5;
      break;
    case SDLK_S:
      transform.y += 5;
      break;
    case SDLK_D:
      transform.x += 5;
      break;
    default:
      break;
    }
  }
}
