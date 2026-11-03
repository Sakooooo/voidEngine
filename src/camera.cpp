#include "camera.h"
#include "base_components.h"
#include <SDL3/SDL_render.h>

void CCamera2DSystem::late_tick(Engine::CEngine* engine, World* world,
                                double deltaTime) {
  int w{0}, h{0};
  if (!SDL_GetCurrentRenderOutputSize(engine->renderer, &w, &h)) {
    spdlog::error("Failed to get the current output's size! {}",
                  SDL_GetError());
    return;
  };

  Vector2 last = engine->view.center;
  engine->view = View2D{}; // incase a camera hasn't been created et

  world->each<Camera2D, Transform2D>(
      [&](Entity entity, Camera2D& camera, Transform2D& transform) {
        if (!camera.current)
          return;

        Vector2 center = transform.center();

        if (camera.follow) {
          if (world->is_valid(*camera.follow) &&
              world->has_component<Transform2D>(camera.follow->entity)) {

            auto& follow_transform =
                world->get_component<Transform2D>(camera.follow->entity);
            center = follow_transform.center();
          } else {
            // Reset since it's outdated
            camera.follow.reset();
            center = last;
            transform.pos = {center.x - transform.width / 2,
                             center.y - transform.height / 2};
          }
        }

        engine->view = {center, {w / 2.0f, h / 2.0f}, camera.zoom};
      });
};
