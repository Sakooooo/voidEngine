#ifndef CAMERA_H
#define CAMERA_H
#include "engine.h"
#include "types.h"
#include "world.h"
#include <SDL3/SDL_rect.h>
#include <optional>
#include <string_view>

struct Camera2D : public Component {
  static constexpr std::string_view name = "Camera2D";
  bool current{false};
  float zoom{1.0f};
  std::optional<EntityReference> follow;
};

class CCamera2DSystem : public ISystem {
  void tick(Engine::CEngine* engine, World* world, double deltaTime) override {
  };
  void late_tick(Engine::CEngine* engine, World* world,
                 double deltaTime) override;
  void draw(Engine::CEngine* engine, World* world) override {};
};

/// Used in draw systems to create a FRect relative to the current view
inline SDL_FRect to_view_FRect(const View2D& view, const Transform2D& t) {
  Vector2 p = view.to_screen(t.pos);
  return {p.x, p.y, t.width * view.zoom, t.height * view.zoom};
}

#endif
