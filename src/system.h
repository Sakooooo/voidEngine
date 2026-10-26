#ifndef SYSTEM_H
#define SYSTEM_H
#include "engine.h"
#include "world.h"
#include <vector>

class ISystem {
public:
  virtual void tick();
};

class CSystemManager {
public:
  CSystemManager(Engine* engine, World& world);

private:
  std::vector<ISystem> m_systems;
};

#endif
