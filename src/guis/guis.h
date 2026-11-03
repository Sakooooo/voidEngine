// Defines all the current internal interfaces
// Add the declaration of new ones here and then create their respective
// interfaced class here
#ifndef GUIS_H
#include "../gui.h"
#include "imgui.h"
#define GUIS_H

#include <array>

class DebugGui : public IGui {
public:
  DebugGui() : IGui() {};
  void render(Engine::CEngine* engine, World* world) override;

private:
  /// Ring buffer of the last frame times in milliseconds
  std::array<float, 120> m_frame_times{};
  std::size_t m_frame_offset{0};
  bool m_show_demo{false};
  bool m_show_metrics{false};
};

class TreeGui : public IGui {
public:
  TreeGui() : IGui() {};
  void render(Engine::CEngine* engine, World* world) override;
};
#endif
