#include "rectangle.h"
#include "camera.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

void CRectSystem::draw(Engine::CEngine* engine, World::CWorld* world) {
  world->each<Rect, Color, Transform2D>([engine](Entity entity, Rect& rectangle,
                                                 Color& color,
                                                 Transform2D& transform) {
    const View2D& view = engine->view;
    SDL_FRect rect = to_view_FRect(view, transform);

    SDL_SetRenderDrawColor(engine->renderer, static_cast<Uint8>(color.r),
                           static_cast<Uint8>(color.g),
                           static_cast<Uint8>(color.b),
                           static_cast<Uint8>(color.a));
    SDL_RenderFillRect(engine->renderer, &rect);
  });
}
