#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "base_components.h"
#include "system.h"
#include "world.h"
#include <string_view>

struct Rect : public Component {
  static constexpr std::string_view name = "Rect";
};

class CRectSystem : public ISystem {
  void tick(Engine* engine, World* world, double deltaTime) override {};
  void draw(Engine* engine, World* world) override;
};

#endif
