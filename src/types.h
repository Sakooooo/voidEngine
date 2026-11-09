#ifndef TYPES_H
#define TYPES_H

#include <cmath>
#include <concepts>
#include <lua.h>
#include <lualib.h>
#include <spdlog/spdlog.h>
#include <string>
#include <type_traits>

template <typename T>
concept NumberConcept = std::integral<T> or std::floating_point<T>;

/// A 2D vector.
struct Vector2 {
  float x, y;

  void operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
  }

  Vector2 operator+(const Vector2& other) const {
    return {
      .x = (x + other.x),
      .y = (y + other.y),
    };
  }

  template <NumberConcept T>
  Vector2 operator*(const T num) const {
    return {static_cast<float>(x * num), static_cast<float>(y * num)};
  }

  Vector2 operator*(const Vector2 other) const {
    return {static_cast<float>(x * other.x), static_cast<float>(y * other.y)};
  }


  void normalize() {
    if (x == 0 && y == 0)
      return;
    auto distance = std::sqrt((std::pow(x, 2) + std::pow(y, 2)));
    x = x / distance;
    y = y / distance;
  };

  /// Multiply (__mul)
  static int lua_mul(lua_State* L) {
    Vector2 result;

    // check each
    if (lua_type(L, 1) == LUA_TNUMBER) {
      // number
      float number = static_cast<float>(lua_tonumber(L, 1));
      auto& vector = *static_cast<Vector2*>(luaL_checkudata(L, 2, "Vector2"));
      result = vector * number;
    } else if (lua_type(L, 2) == LUA_TNUMBER) {
      // other way around
      auto& vector = *static_cast<Vector2*>(luaL_checkudata(L, 1, "Vector2"));
      float number = static_cast<float>(lua_tonumber(L, 2));
      result = vector * number;
    } else {
      // vector
      auto& a = *static_cast<Vector2*>(luaL_checkudata(L, 1, "Vector2"));
      auto& b = *static_cast<Vector2*>(luaL_checkudata(L, 2, "Vector2"));
      result = a * b;
    }

    Vector2* vector =
      static_cast<Vector2*>(lua_newuserdata(L, sizeof(Vector2)));

    *vector = result;

    luaL_getmetatable(L, "Vector2");
    lua_setmetatable(L, -2);

    return 1;
  }

  /// Add (__add)
  static int lua_add(lua_State* L) {
    auto& a = *static_cast<Vector2*>(luaL_checkudata(L, 1, "Vector2"));
    auto& b = *static_cast<Vector2*>(luaL_checkudata(L, 2, "Vector2"));

    Vector2* vector =
      static_cast<Vector2*>(lua_newuserdata(L, sizeof(Vector2)));

    *vector = a + b;

    luaL_getmetatable(L, "Vector2");
    lua_setmetatable(L, -2);

    return 1;
  };

  /// Index (__index)
  static int lua_index(lua_State* L) {
    auto& vector = *static_cast<Vector2*>(luaL_checkudata(L, 1, "Vector2"));
    std::string key = static_cast<std::string>(luaL_checkstring(L, 2));

    if (key == "x") {
      lua_pushnumber(L, vector.x);
    } else if (key == "y") {
      lua_pushnumber(L, vector.y);
    } else {
      // lua_pushnil(L);
      lua_getfield(L, lua_upvalueindex(1), key.c_str());
    }

    return 1;
  };

  /// vector:normalize(), returns Vector2 (just calls Normalize)
  static int lua_normalize(lua_State* L) {
    auto target = *static_cast<Vector2*>(luaL_checkudata(L, 1, "Vector2"));
    target.normalize();

    Vector2* vector =
      static_cast<Vector2*>(lua_newuserdata(L, sizeof(Vector2)));
    *vector = target;

    luaL_getmetatable(L, "Vector2");
    lua_setmetatable(L, -2);

    return 1;
  }

  /// Vector2.new(x, y), returns Vector2 = { x: number, y: number }
  static int lua_new(lua_State* L) {
    float x = static_cast<float>(luaL_checknumber(L, 1));
    float y = static_cast<float>(luaL_checknumber(L, 2));

    spdlog::trace("Created new Vector2 with x: {}, y: {}", x, y);

    // TODO: use luau userdata tags on these
    Vector2* vector =
	static_cast<Vector2*>(lua_newuserdata(L, sizeof(Vector2)));

    *vector = {x, y};

    luaL_getmetatable(L, "Vector2");
    lua_setmetatable(L, -2);

    return 1;
  };
};

/// A global 2D view, used when in 2D and adjusts to the Camera2D
struct View2D {
  Vector2 center{0, 0};
  Vector2 screen_center{0, 0};
  float zoom{1.0f};

  /// Return the position of another object relative to the View2D
  Vector2 to_screen(Vector2 other) const {
    return {(other.x - center.x) * zoom + screen_center.x,
	    (other.y - center.y) * zoom + screen_center.y};
  }
};

#endif
