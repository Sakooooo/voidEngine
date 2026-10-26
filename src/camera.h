#ifndef CAMERA_H
#define CAMERA_H
#include "engine.h"
#include "types.h"
#include "world.h"

namespace Camera2D {

struct Camera2D : public Component {
  Vector2 pos;
  int width, height;
  bool current;
  bool follow{false};
};
} // namespace Camera2D

#endif
