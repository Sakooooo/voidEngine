#include "system.h"
#include <spdlog/spdlog.h>

CSystemManager::CSystemManager(Engine* engine, World* world) {
  spdlog::trace("constructing CSystemManager");
  this->engine = engine;
  this->world = world;
}

void CSystemManager::tick(double deltaTime) {
  for (auto& system : m_systems) {
    system->tick(engine, world, deltaTime);
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

// void TestSystem::tick(Engine* engine, World* world) { printf("I'm ticking!");
// };
