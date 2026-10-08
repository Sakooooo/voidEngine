#include "world.h"
#include "camera.h"
#include "imgui.h"
#include <cmath>

void mySystem(SDL_Renderer* r) {
  auto& world{World::get_instance()};

  // TODO: This is really dumb, it might be better to make a Camera struct that
  // contains an Entity and an actual Camera2D struct and grab it from world as
  // there can only be one camera in a World.
  // update: I think it should just rely on a current variable?? Pointer to
  // camera maybe?

  Camera2D* camera;

  for (const auto e : world.get_entities()) {
    auto camera_comp = world.view<Camera2D>(e);
    if (!camera_comp)
      continue;

    auto [myCamera] = *camera_comp;
    if (myCamera.current) {
      camera = &myCamera;
      break;
    }
  }

  for (const auto e : world.get_entities()) {
    auto comps = world.view<Transform, Color>(e);
    if (!comps)
      continue;

    auto [transform, color] = *comps;
    SDL_FRect rect;

    if (camera) {
      rect = {transform.pos.x - camera->pos.x, transform.pos.y - camera->pos.y,
	      transform.width, transform.height};
    } else {
      rect = {transform.pos.x, transform.pos.y, transform.width,
	      transform.height};
    };

    SDL_SetRenderDrawColor(
	r, static_cast<Uint8>(color.r), static_cast<Uint8>(color.g),
	static_cast<Uint8>(color.b), static_cast<Uint8>(color.a));
    SDL_RenderFillRect(r, &rect);
  }
}

void funnyRainbowSystem(double dt) {
  auto& world{World::get_instance()};
  for (const auto e : world.get_entities()) {
    auto comps = world.view<Color, Rainbow>(e);
    if (!comps)
      continue;

    auto [color, rainbow] = *comps;

    rainbow.hue = std::fmod(rainbow.hue + rainbow.speed * dt, 1.0f);

    float r, g, b;
    ImGui::ColorConvertHSVtoRGB(rainbow.hue, 1.0f, 1.0f, r, g, b);
    color.r = static_cast<int>(r * 255);
    color.g = static_cast<int>(g * 255);
    color.b = static_cast<int>(b * 255);
  }
}
