// I gotta rework this later :(
#ifndef WORLD_H
#define WORLD_H
#include "base_components.h"
#include "engine.h"
#include "types.h"
#include <SDL3/SDL_render.h>
#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <typeindex>
#include <unordered_map>
#include <vector>

/// Store entities as a regular integer
using Entity = std::uint32_t;

/// Concept to be used for checking if they're components or not (derived from
/// Component)
template <typename ComponentType>
concept ComponentConcept =
    std::derived_from<ComponentType, Component> && requires {
      { ComponentType::name } -> std::convertible_to<std::string_view>;
    };

/// Class that is used by Storage to be able to store component stores in single
/// containers like std::vector
class IStorage {
public:
  virtual ~IStorage() = default;
  virtual void remove(Entity entity) = 0;
  virtual bool has(Entity entity) const = 0;
  virtual std::string_view name() const = 0;
};

/// Stores all components for entities of type ComponentType that matches the
/// ComponentConcept (anything derived from the Component struct)
template <ComponentConcept ComponentType>
class Storage : public IStorage {
public:
  /// Add an Entity and it's Component to the Storage
  void append(Entity entity, ComponentType component) {
    m_storage[entity] = component;
  }

  /// Grab the specific Component for an Entity
  ComponentType& get(Entity entity) {
    auto it = m_storage.find(entity);
    assert(it != m_storage.end() && "Entity does not have this component.");
    return it->second;
  }

  /// Check if the Storage contains the Entity
  bool has(Entity entity) const override {
    return m_storage.find(entity) != m_storage.end();
  }

  /// Remove an entity from the Storage
  void remove(Entity entity) override { m_storage.erase(entity); }

  std::string_view name() const override { return ComponentType::name; }

  /// Returns reference to Storage's internal m_storage
  std::unordered_map<Entity, ComponentType>& all() { return m_storage; }

private:
  std::unordered_map<Entity, ComponentType> m_storage{};
};

/// Controls the entire ECS
class World {
public:
  /// Create a new entity, which is just another ID
  /// Checks for any available free entities in the m_free vector,
  /// otherwise it will increment the m_nextId variable
  Entity create_entity() {
    spdlog::trace("Creating new entity");
    Entity id;
    if (!m_free.empty()) {
      id = m_free.back();
      m_free.pop_back();
    } else {
      id = m_nextId++;
    }
    m_entities.push_back(id);
    if (id >= m_entity_alive.size()) {
      m_entity_alive.resize(id + 1);
    }
    m_entity_alive[id] = true;
    return id;
  }

  /// Destroy an entity, removing it from all Storages and adding it to the
  /// m_free list for later reuse
  void destroy_entity(Entity entity) {
    spdlog::trace("Destroying entity");
    if (!is_alive(entity)) {
      spdlog::trace("Entity is already dead");
      return;
    }
    for (auto& [typeIndex, array] : m_storages) {
      spdlog::trace("removing entity from stores");
      array->remove(entity);
    }
    for (auto it = m_entities.begin(); it != m_entities.end(); ++it) {
      if (*it == entity) {
	spdlog::trace("erase entity");
	m_entities.erase(it);
	break;
      }
    }
    m_entity_alive[entity] = false;
    m_free.push_back(entity);
  }

  bool is_alive(Entity entity) const {
    return entity < m_entity_alive.size() && m_entity_alive[entity];
  }

  /// Create or get the Storage of ComponentType
  template <ComponentConcept ComponentType>
  Storage<ComponentType>& get_storage_array() {
    std::type_index typeIdx(typeid(ComponentType));
    if (m_storages.find(typeIdx) == m_storages.end()) {
      m_storages[typeIdx] = std::make_unique<Storage<ComponentType>>();
    }
    return *static_cast<Storage<ComponentType>*>(m_storages[typeIdx].get());
  }

  /// Adds a component to an Entity
  template <ComponentConcept ComponentType>
  ComponentType& add_component(Entity entity, ComponentType component) {
    auto& array = get_storage_array<ComponentType>();
    array.append(entity, component);
    return array.get(entity);
  }

  /// Check if an Entity has a specific component
  template <ComponentConcept ComponentType>
  bool has_component(Entity entity) {
    return get_storage_array<ComponentType>().has(entity);
  }

  /// Return the list of all entities
  const std::vector<Entity>& all_entities() const { return m_entities; }

  /// Get all the components NAMES that an entity has as a std::vector of
  /// std::string
  std::vector<std::string> get_all_component_names(Entity entity) {
    auto it = std::find(m_entities.begin(), m_entities.end(), entity);
    assert(it != m_entities.end() && "Entity does not exist");
    assert(m_entity_alive[entity] && "Entity is not alive!");

    std::vector<std::string> result_components;

    for (auto& it : m_storages) {
      if (it.second->has(entity)) {
	result_components.emplace_back(it.second->name());
      }
    }

    return result_components;
  }

  /// Get a specific component of a specific entity
  template <ComponentConcept ComponentType>
  ComponentType& get_component(Entity entity) {
    return get_storage_array<ComponentType>().get(entity);
  }

  /// Run a function for every entity which meets the following template
  /// Pass it in like [](Entity entity, ComponentType& Component...)
  template <ComponentConcept... Components, typename Func>
  void each(Func func) {
    for (Entity entity : m_entities) {
      bool hasAll = (has_component<Components>(entity) && ...);
      if (hasAll) {
	func(entity, get_component<Components>(entity)...);
      }
    }
  }

  template <ComponentConcept... Components>
  void remove_componet(Entity entity) {
    spdlog::trace("remove_component");
    if ((has_component<Components>(entity) && ...))
      (get_storage_array<Components>().remove(entity), ...);
  }

private:
  /// The next ID to assign
  Entity m_nextId = 0;
  /// The list of live entities
  std::vector<Entity> m_entities{};
  std::vector<bool> m_entity_alive{};
  std::vector<Entity> m_free{};
  /// List of all Components and their respective Storages
  std::unordered_map<std::type_index, std::unique_ptr<IStorage>> m_storages;
};

// void mySystem(SDL_Renderer* r);

// void funnyRainbowSystem(double dt);o

#endif
