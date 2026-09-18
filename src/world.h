// I gotta rework this later :(
#ifndef WORLD_H
#define WORLD_H
#include "engine.h"
#include <SDL3/SDL_render.h>
#include <concepts>
#include <cstdint>
#include <limits>
#include <memory>
#include <optional>
#include <tuple>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

using Entity = std::uint32_t;

struct Component {};

template <typename ComponentType>
concept ComponentConcept = std::derived_from<ComponentType, Component>;

// to do this, just define void onLoad(Entity e)
template <typename ComponentType>
concept HasOnLoad = requires(ComponentType& component, Engine* engine, Entity e) { component.onLoad(engine, e); }; // not sure if this is a bad idea

struct Transform : public Component {
  float x{};
  float y{};
};

struct Color : public Component {
  int r{};
  int g{};
  int b{};
  int a{};
};

struct Rainbow : public Component {
  float speed{1.0f};
  float hue{0.0f};
};

template <ComponentConcept ComponentType>
class SparseSet {
public:
  ComponentType& insert(Entity key, const ComponentType& component) {
    ensure_sparse_size(key);

    if (m_sparse[key] == INVALID) {
      m_sparse[key] = m_dense.size();
      m_dense_keys.push_back(key);
      m_dense.push_back(component);
    } else {
      m_dense[m_sparse[key]] = component;
    }

    return m_dense[m_sparse[key]];
  }

  void remove(Entity key) {
    if (!contains(key))
      return;

    auto index = m_sparse[key];
    auto last = m_dense.size() - 1;

    m_dense[index] = std::move(m_dense[last]);
    m_dense_keys[index] = m_dense_keys[last];
    m_sparse[m_dense_keys[index]] = index;

    m_dense.pop_back();
    m_dense_keys.pop_back();
    m_sparse[key] = INVALID;
  }

  bool contains(Entity key) const {
    return key < m_sparse.size() && m_sparse[key] != INVALID;
  }

  ComponentType* get(Entity key) {
    if (!contains(key))
      return nullptr;
    return &m_dense[m_sparse[key]];
  }

  // This is to get all of the entities.
  const auto& get_entities() const { return m_dense_keys; }

private:
  void ensure_sparse_size(Entity key) {
    if (key >= m_sparse.size())
      m_sparse.resize(key + 1, INVALID);
  }
  static constexpr std::size_t INVALID{std::numeric_limits<std::size_t>::max()};
  std::vector<std::size_t> m_sparse{};
  std::vector<Entity> m_dense_keys{};
  std::vector<ComponentType> m_dense{};
};

struct IStorage {
  virtual ~IStorage() = default;
  virtual void remove_if_present(Entity) = 0;
};

template <ComponentConcept ComponentType>
class Storage : public IStorage {
public:
  void remove_if_present(Entity e) override { m_storage.remove(e); }

  template <typename... Args>
  ComponentType& add_component(Entity e, Args&&... args) {
    return m_storage.insert(e, ComponentType{{}, std::forward<Args>(args)...});
  }

  ComponentType* get_component(Entity e) { return m_storage.get(e); }

  void remove_component(Entity e) { m_storage.remove(e); }

  const auto& get_entities() const { return m_storage.get_entities(); }

private:
  SparseSet<ComponentType> m_storage{};
};

class World {
public:
  static World& get_instance() {
    static World instance{};
    return instance;
  }

  void set_engine(Engine& engine) { m_engine = &engine; }

  Engine& engine() {
    return *m_engine;
  }

  template <ComponentConcept ComponentType>
  Storage<ComponentType>& get_storage() {
    auto [it, inserted] =
	m_storages.try_emplace(std::type_index(typeid(ComponentType)), nullptr);
    if (inserted)
      it->second = std::make_unique<Storage<ComponentType>>();
    return *static_cast<Storage<ComponentType>*>(it->second.get());
  }

  template <ComponentConcept ComponentType, typename... Args>
  ComponentType& add_component(Entity e, Args&&... args) {
    auto& storage{get_storage<ComponentType>()};
    auto& component = storage.add_component(e, std::forward<Args>(args)...);
    if constexpr (HasOnLoad<ComponentType>)
      component.onLoad(m_engine, e);
    return component;
  }

  template <ComponentConcept ComponentType>
  void remove_component(Entity e) {
    auto& storage{get_storage<ComponentType>()};
    storage.remove_component(e);
  }

  template <ComponentConcept ComponentType>
  ComponentType* get_component(Entity e) {
    auto& storage{get_storage<ComponentType>()};
    return storage.get_component(e);
  }

  // Returns references to all of the requested components if the entity has
  // every one of them, otherwise nullopt. The references point into the
  // component storages, so don't hold on to them across an add_component of
  // the same type.
  template <ComponentConcept... Components>
  std::optional<std::tuple<Components&...>> view(Entity e) {
    std::tuple<Components*...> ptrs{get_component<Components>(e)...};
    if (!std::apply([](auto*... p) { return (p && ...); }, ptrs))
      return std::nullopt;
    return std::apply(
	[](auto*... p) { return std::tuple<Components&...>(*p...); }, ptrs);
  }

  const std::vector<Entity>& get_entities() const { return m_entities; }

  Entity createEntity() {
    Entity newEntity{nextId++};
    m_entities.push_back(newEntity);
    return newEntity;
  }

  void destroyEntity(Entity e) {
    for (auto& [type, storage] : m_storages)
      storage->remove_if_present(e);
    std::erase(m_entities, e);
  }

private:
  // prevent new world
  World() = default;
  Entity nextId = 0;
  std::vector<Entity> m_entities{};
  std::unordered_map<std::type_index, std::unique_ptr<IStorage>> m_storages{};
  Engine* m_engine{nullptr};
};

void mySystem(SDL_Renderer* r);

void funnyRainbowSystem(double dt);

#endif
