#include "system.h"

CSystemManager::CSystemManager(Engine* engine, World* world) {
  this->engine = engine;
  this->world = world;
}

void CSystemManager::tick() {
  for (auto& system : m_systems) {
    system->tick(engine, world);
  }
}

/// Add a system
void CSystemManager::add(ISystem* system) { m_systems.push_back(system); }

/// Remove a system
void CSystemManager::remove(ISystem* system) {
  // Search through the entire vector for the system to remove
  m_systems.erase(std::remove(m_systems.begin(), m_systems.end(), system));
}

// void TestSystem::tick(Engine* engine, World* world) { printf("I'm ticking!");
// };
