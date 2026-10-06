#include "camera.h"

Vector2 Camera::updateFollowCords(Vector2 pos, Size size, Camera2D camera) {

  return Vector2{.x = pos.x + size.width / 2 - camera.width / 2,
		 .y = pos.y + size.height / 2 - camera.height / 2};
}

void Camera::Camera2DSystem() {
  auto& world = World::get_instance();

  for (const auto e : world.get_entities()) {
    auto query{world.view<Camera2D, Transform, Size>(e)};

    if (!query)
      continue;

    auto [camera, transform, size] = *query;

    if (camera.follow)
      camera.pos = Camera::updateFollowCords(transform.pos, size, camera);
  }
}
