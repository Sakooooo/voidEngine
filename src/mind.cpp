// IT'S GOT A MIND OF IT'S OWN OH GOD
#include "mind.h"
#include "camera.h"
#include <SDL3/SDL_keyboard.h>

void mindSystem(const bool* keyboard_state) {
  auto& world{World::get_instance()};

  for (const auto e : world.get_entities()) {
    auto query{world.view<Controllable, Transform>(e)};
    if (!query)
      continue;

    auto [controllable, transform] = *query;
    if (!controllable.mind.controlled)
      continue;

    controllable.forward = keyboard_state[SDL_SCANCODE_W];
    controllable.left = keyboard_state[SDL_SCANCODE_A];
    controllable.backward = keyboard_state[SDL_SCANCODE_S];
    controllable.right = keyboard_state[SDL_SCANCODE_D];
  }
}

void ControllableSystem() {
  auto& world{World::get_instance()};

  for (const auto e : world.get_entities()) {
    auto query{world.view<Controllable, Transform>(e)};
    if (!query)
      continue;

    auto [controllable, transform] = *query;
    if (!controllable.mind.controlled)
      continue;

    if (controllable.forward)
      transform.y -= 5;
    if (controllable.left)
      transform.x -= 5;
    if (controllable.backward)
      transform.y += 5;
    if (controllable.right)
      transform.x += 5;

    auto camera_query{world.view<Camera2D>(e)};

    auto [camera] = *camera_query;

    camera.x = transform.x + 250.0f / 2 - camera.width / 2;
    camera.y = transform.y + 250.0f / 2 - camera.height / 2;
  }
}
