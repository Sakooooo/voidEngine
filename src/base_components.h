/// This file includes all the base components needed for uhh stuff
/// Put simple shit in here
#ifndef BASE_COMPONENTS_H
#define BASE_COMPONENTS_H
#include "engine.h"
#include "lua.h"
#include "lualib.h"
#include "types.h"
#include <string_view>

/// The base component struct, you should NOT be using this as its own!
struct Component {};

/// A component that describes the Transformations of a 2D object
struct Transform2D : public Component {
  static constexpr std::string_view name = "Transform2D";
  Vector2 pos;
  float width;
  float height;

  /// Return a vector of the calculated center of the Transform2D
  Vector2 center() const { return {pos.x + width / 2, pos.y + height / 2}; }
};

/// A component that describes the Color of a 2D/3D object
struct Color : public Component {
  static constexpr std::string_view name = "Color";
  int r, g, b, a;
};

#endif
