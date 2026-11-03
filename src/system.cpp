#include "system.h"
#include <spdlog/spdlog.h>

CSystemManager::CSystemManager(Engine::CEngine* engine, World::CWorld* world) {
  spdlog::trace("constructing CSystemManager");
  this->engine = engine;
  this->world = world;
}

void CSystemManager::tick(double deltaTime) {
  for (auto& system : m_systems) {
    system->tick(engine, world, deltaTime);
  }
  for (auto& system : m_systems) {
    system->late_tick(engine, world, deltaTime);
  }
}

void CSystemManager::draw() {
  for (auto& system : m_systems) {
    system->draw(engine, world);
  }
}

/// Add a system
void CSystemManager::add(ISystem* system) {
  spdlog::trace("add system to CSystemManager");
  m_systems.push_back(system);
}

/// Remove a system
void CSystemManager::remove(ISystem* system) {
  spdlog::trace("remove system to CSystemManager");
  std::erase(m_systems, system);
}

// void TestSystem::tick(Engine::CEngine* engine, World::CWorld* world, double
// deltaTime) {
//   printf("I'm ticking!");
// };

// void TestSystem::draw(Engine::CEngine* engine, World::CWorld* world) {
// printf("draw"); }
