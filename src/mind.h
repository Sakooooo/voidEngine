// IT'S GOT A MIND OF IT'S OWN OH GOD
#ifndef MIND_H
#define MIND_H
#include "SDL3/SDL_events.h"
#include "world.h"

// i want this to be some sort of identifier so i can assign it to things that
// can control it
class Mind {
public:
  bool controlled = false;
};

struct Controllable : public Component {
  Mind *mind;
};

void mindSystem(SDL_Event event);
#endif