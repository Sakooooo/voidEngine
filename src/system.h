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

  // these should be kept as 0 because it'll be more clear when systems don't do
  // a specific kind of thing "self-documenting" :D

  // TODO: I want to replace this with a dependencies kind of ticking
  //
  // #-
  // Probably add a depends on vector?
  // Then have a list of systems that haven't ticked and possibly cause it to
  // check if that system has ticked before running every frame, but that might
  // be too slow and will cause it to be slow as shit
  // Maybe I'll stick to this? Not sure either way since it isn't a priority right now
  //
  /// Tick as soon as possible
  virtual void tick(Engine* engine, World* world, double deltaTime) = 0;
  /// Tick once early ticks have passed
  virtual void late_tick(Engine* engine, World* world, double deltaTime) = 0;
  virtual void draw(Engine* engine, World* world) = 0;
};

// class TestSystem : public ISystem {
//   void tick(Engine* engine, World* world, double deltaTime) override;
//   void draw(Engine* engine, World* world) override;
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
  void draw();

private:
  /// Private vector store that holds pointers to all currently loaded systems
  std::vector<ISystem*> m_systems;
};

#endif
