// Let's make games in SmileBASIC!
#ifndef SCRIPT_H
#define SCRIPT_H

#include "world.h"
struct Script : public Component {
  char* filepath;

  void onLoad() override {}
};

struct SharedBool : public Component {
  bool value;
};

#endif
