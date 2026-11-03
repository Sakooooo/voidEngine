#include "player_controller.h"

void CPlayerControllerSystem::tick(Engine::CEngine* engine, World* world,
                                   double deltaTime) {

  const bool* keys = engine->keyboard_state;
  world->each<Transform2D, PlayerController>(
      [keys, deltaTime](Entity entity, Transform2D& transform,
                        PlayerController& controller) {
        controller.up = keys[SDL_SCANCODE_W];
        controller.down = keys[SDL_SCANCODE_S];
        controller.left = keys[SDL_SCANCODE_A];
        controller.right = keys[SDL_SCANCODE_D];

        Vector2 dir{0, 0};

        if (controller.up)
          dir.y -= 1;
        if (controller.down)
          dir.y += 1;
        if (controller.left)
          dir.x -= 1;
        if (controller.right)
          dir.x += 1;

        dir.normalize();

        transform.pos += dir * 200 * deltaTime;
      });
};
