#include "guis.h"

void TestGui::render(Engine* engine, World* world) {
  ImGui::Begin("TestGui");
  ImGui::Text("Test");
  ImGui::End();
}
