#include "script.h"
#include <lua.h>
#include <lualib.h>

void scriptTickSystem(Engine* engine, double dt) {
  auto& world{World::get_instance()};

  for (const auto e : world.get_entities()) {
    auto query{world.view<Script>(e)};
    if (!query)
      continue;

    auto [script] = *query;

    if (!script.initalized)
      continue;

    if (script.tickRef == LUA_REFNIL)
      continue;
  }
}
