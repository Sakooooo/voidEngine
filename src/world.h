#ifndef WORLD_H
#define WORLD_H
#include <concepts>
#include <cstdint>
#include <unordered_map>
#include <vector>

using Entity = std::uint32_t;

struct Component {};

template <typename ComponentType>
concept ComponentConcept = std::derived_from<ComponentType, Component>;

struct Transform : public Component {
  float x;
  float y;
};

struct Something : public Component {
  int something;
};

template <ComponentConcept ComponentType> class Storage {
public:
  template <typename... Args>
  ComponentType &add_component(Entity e, Args &&...args) {
    auto &[it, inserted] =
        m_storage.emplace(e, ComponentType{{}, std::forward<Args>(args)...});
    return it->second;
  };

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

  Entity createEntity() { return nextId++; };

private:
  // prevent new world
  World() = default;
  Entity nextId = 0;
};

#endif
