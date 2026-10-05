#include "camera.h"

void Camera2DSystem() {
  auto& world = World::get_instance();

  for (const auto e : world.get_entities()) {
    auto query{world.view<Camera2D>(e)};

    if (!query)
      continue;

    auto [camera] = *query;
  }
}
