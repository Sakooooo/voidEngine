// IT'S GOT A MIND OF IT'S OWN OH GOD
#ifndef MIND_H
#define MIND_H
#include "world.h"

// i want this to be some sort of identifier so i can assign it to things that
// can control it
class Mind {
public:
  bool controllable = false;
};

struct Controllable : public Component {
  Mind *mind;
};

void mindSystem() {
  auto &world{World::get_instance()};

  for (const auto e : world.get_entities()) {
    auto controllable_query{world.view<Controllable>(e)};

    if (auto controllable_result = controllable_query) {
      auto [controllable] = *controllable_result;

      if (controllable.mind->controllable) {
      }
    }
  }
}

#endif