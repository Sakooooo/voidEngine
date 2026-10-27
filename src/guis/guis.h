// Defines all the current internal interfaces
// Add the declaration of new ones here and then create their respective
// interfaced class here
#ifndef GUIS_H
#include "../gui.h"
#include "imgui.h"
#define GUIS_H

class TestGui : public IGui {
public:
  TestGui(const char* name = "test") : IGui(name) {};
  void render() override;
};
#endif
