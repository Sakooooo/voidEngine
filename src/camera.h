#ifndef CAMERA_H
#define CAMERA_H
#include "engine.h"
#include "types.h"
#include "world.h"

struct Camera2D : public Component {
  Vector2 pos;
  int width, height;
  bool current;
  bool follow{false};
};

namespace Camera {

Vector2 updateFollowCords(Transform transform, Camera2D camera);
void Camera2DSystem();
void UpdateCameraSize(Engine* engine);
} // namespace Camera

#endif
