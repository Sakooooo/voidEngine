#ifndef SYSTEM_H
#define SYSTEM_H
#include <memory>
#include <vector>

// forward defs
class Engine;
class World;

/// An interface class that is used to define new systems
class ISystem {
public:
  virtual ~ISystem() = default;
  virtual void tick(Engine* engine, World* world, double deltaTime) = 0;
};

// class TestSystem : public ISystem {
//   void tick(Engine* engine, World* world) override;
// };

/// A class that handles management of all systems
class CSystemManager {
public:
  CSystemManager(Engine* engine, World* world);

  // Construct with engine and world to pass through into tick()
  Engine* engine;
  World* world;

  void add(ISystem* system);
  void remove(ISystem* system);

  void tick(double deltaTime);

private:
  /// Private vector store that holds pointers to all currently loaded systems
  std::vector<ISystem*> m_systems;
};

#endif
