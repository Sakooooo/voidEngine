#ifndef SCRIPT_H
#define SCRIPT_H
#include "lua.h"
#include "world.h"
#include <string>
#include <string_view>
#include <unordered_map>

struct LuauScript : public Component {
  static constexpr std::string_view name = "LuauScript";
  std::string path;
  bool reload{false};
};

class CLuauScriptSystem : public ISystem {
  struct ScriptInstance {
    EntityReference ref{};
    std::string path{};
    int environment_ref{LUA_NOREF};
    bool failed{false};
  };
  std::unordered_map<Entity, ScriptInstance> m_instances{};
  void tick(Engine::CEngine* engine, World* world, double deltaTime) override;
  void late_tick(Engine::CEngine* engine, World* world,
                 double deltaTime) override {};
  void draw(Engine::CEngine* engine, World* world) override {};

  static bool load_script(lua_State* L, Entity entity, const std::string& path,
                          int& environment_ref);
};

#endif
