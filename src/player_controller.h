#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "world.h"
#include <string_view>

struct PlayerController : public Component {
  static constexpr std::string_view name = "PlayerController";
  bool up{false}, down{false}, left{false}, right{false};
};

class CPlayerControllerSystem : public ISystem {
  void tick(Engine* engine, World* world, double deltaTime) override;
  void late_tick(Engine* engine, World* world, double deltaTime) override {};
  void draw(Engine* engine, World* world) override {};
};

#endif
