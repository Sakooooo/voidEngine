#ifndef WORLD_H
#define WORLD_H
#include <cstdint>
#include <vector>

using Entity = std::uint32_t;

class World {
public:
  Entity createEntity() { return nextId++; }

private:
  Entity nextId = 0;
};

#endif
