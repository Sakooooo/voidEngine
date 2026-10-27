/// This file includes all the base components needed for uhh stuff
/// Put simple shit in here
#ifndef BASE_COMPONENTS_H
#define BASE_COMPONENTS_H
#include "types.h"

/// The base component struct, you should NOT be using this as its own!
struct Component {};

/// A component that describes the Transformations of a 2D object
struct Transform2D : public Component {
  Vector2 pos;
  float width;
  float height;
};

/// A component that describes the Color of a 2D/3D object
struct Color : public Component {
  int r, g, b, a;
};

#endif
