#include "camera.h"

Vector2 Camera::updateFollowCords(Transform transform, Camera2D camera) {

  return Vector2{.x = transform.pos.x + transform.width / 2 - camera.width / 2,
		 .y = transform.pos.y + transform.height / 2 -
		     camera.height / 2};
}

// TODO: Handle multiple cameras that aren't focused
void Camera::Camera2DSystem() {
  auto& world = World::get_instance();

  for (const auto e : world.get_entities()) {
    auto query{world.view<Camera2D, Transform>(e)};

    if (!query)
      continue;

    auto [camera, transform] = *query;

    if (camera.follow)
      camera.pos = Camera::updateFollowCords(transform, camera);
  }
}

void Camera::UpdateCameraSize(Engine* engine) {
  auto& world = World::get_instance();

  for (const auto e : world.get_entities()) {
    auto query{world.view<Camera2D>(e)};

    if (!query)
      continue;

    auto [camera] = *query;

    if (camera.current)
      SDL_GetWindowSize(engine->window, &camera.width, &camera.height);
  }
}
