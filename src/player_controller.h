#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "world.h"
#include <string_view>

struct PlayerController : public Component {
  static constexpr std::string_view name = "PlayerController";
  bool up{false}, down{false}, left{false}, right{false};
};

class CPlayerControllerSystem : public ISystem {
  void tick(Engine::CEngine* engine, World::CWorld* world,
            double deltaTime) override;
  void late_tick(Engine::CEngine* engine, World::CWorld* world,
                 double deltaTime) override {};
  void draw(Engine::CEngine* engine, World::CWorld* world) override {};
};

#endif
