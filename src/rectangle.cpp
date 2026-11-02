#include "rectangle.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

void CRectSystem::draw(Engine* engine, World* world) {
  world->each<Rect, Color, Transform2D>([engine](Entity entity, Rect& rectangle,
						 Color& color,
						 Transform2D& transform) {
    SDL_FRect rect{.x = transform.pos.x,
		   .y = transform.pos.y,
		   .w = transform.width,
		   .h = transform.height};

    SDL_SetRenderDrawColor(engine->renderer, static_cast<Uint8>(color.r),
			   static_cast<Uint8>(color.g),
			   static_cast<Uint8>(color.b),
			   static_cast<Uint8>(color.a));
    SDL_RenderFillRect(engine->renderer, &rect);
  });
}
