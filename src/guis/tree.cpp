#include "guis.h"

void TreeGui::render(Engine* engine, World* world) {
  ImGui::Begin("Test");
  ImGui::Text("This is the tree!");
  ImGui::End();
}
