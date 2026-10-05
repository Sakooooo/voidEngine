#ifndef CAMERA_H
#define CAMERA_H
#include "world.h"

struct Camera2D : public Component {
  float x, y;
  int width, height;
};

void Camera2DSystem();

#endif
