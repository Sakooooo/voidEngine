// IT'S GOT A MIND OF IT'S OWN OH GOD
#ifndef MIND_H
#define MIND_H
#include "world.h"
#include <SDL3/SDL_events.h>

// i want this to be some sort of identifier so i can assign it to things that
// can control it
struct Mind {
  bool controlled = false;
};

struct Controllable : public Component {
  Mind mind{};
  bool forward{false};
  bool backward{false};
  bool left{false};
  bool right{false};
};

void mindSystem(const bool* keyboard_state);
void ControllableSystem();
#endif
