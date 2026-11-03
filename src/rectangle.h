#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "base_components.h"
#include "system.h"
#include "world.h"
#include <string_view>

/// Literally just exists to display a rectangle
struct Rect : public Component {
  static constexpr std::string_view name = "Rect";
};

/// System to draw all entities with a rectangle
class CRectSystem : public ISystem {
  void tick(Engine::CEngine* engine, World::CWorld* world,
            double deltaTime) override {};
  void late_tick(Engine::CEngine* engine, World::CWorld* world,
                 double deltaTime) override {};
  void draw(Engine::CEngine* engine, World::CWorld* world) override;
};

#endif
