#ifndef WORLD_H
#define WORLD_H
#include <SDL3/SDL_render.h>
#include <concepts>
#include <cstdint>
#include <functional>
#include <optional>
#include <unordered_map>
#include <vector>

using Entity = std::uint32_t;

struct Component {};

template <typename ComponentType>
concept ComponentConcept = std::derived_from<ComponentType, Component>;

struct Transform : public Component {
  float x{};
  float y{};
};

struct Something : public Component {
  int something;
};

template <ComponentConcept ComponentType> class Storage {
public:
  template <typename... Args>
  ComponentType &add_component(Entity e, Args &&...args) {
    auto [it, inserted] =
	m_storage.emplace(e, ComponentType{{}, std::forward<Args>(args)...});
    return it->second;
  };

  std::optional<std::reference_wrapper<ComponentType>> get_component(Entity e) {
    auto it{m_storage.find(e)};
    if (it == m_storage.end())
      return std::nullopt;

    return it->second;
  }

  auto &get_map() { return m_storage; }

private:
  std::unordered_map<Entity, ComponentType> m_storage{};
};

class World {
public:
  static World &get_instance() {
    static World instance{};
    return instance;
  };

  template <ComponentConcept ComponentType>
  Storage<ComponentType> &get_storage() {
    static Storage<ComponentType> storage{};
    return storage;
  };

  template <ComponentConcept ComponentType, typename... Args>
  ComponentType &add_component(Entity e, Args &&...args) {
    auto &storage{get_storage<ComponentType>()};
    return storage.add_component(e, std::forward<Args>(args)...);
  };

  template <ComponentConcept ComponentType>
  std::optional<std::reference_wrapper<ComponentType>> get_component(Entity e) {
    auto &storage{get_storage<ComponentType>()};
    return storage.get_component(e);
  };

  template <ComponentConcept ComponentType> auto &get_component_map() {
    return get_storage<ComponentType>().get_map();
  }

  Entity createEntity() { return nextId++; };

private:
  // prevent new world
  World() = default;
  Entity nextId = 0;
};

void mySystem(SDL_Renderer *r) {
  auto &world{World::get_instance()};

  for (const auto &[e, transform_component] :
       world.get_component_map<Transform>()) {
    auto transform{world.get_component<Transform>(e)};
    if (!transform.has_value())
      continue;
    SDL_FRect rect{transform->get().x, transform->get().y, 500, 500};
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderFillRect(r, &rect);
  }
}

#endif
