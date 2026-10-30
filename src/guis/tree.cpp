#include "guis.h"
#include "imgui.h"
#include "world.h"

void TreeGui::render(Engine* engine, World* world) {
  ImGui::Begin("Test");
  ImGui::Text("This is the tree!");

  if (ImGui::TreeNodeEx("Entities", ImGuiTreeNodeFlags_DefaultOpen)) {
    for (auto entity : world->all_entities()) {
      ImGui::PushID(entity);
      if (ImGui::TreeNodeEx(std::to_string(entity).c_str(),
			    ImGuiTreeNodeFlags_Leaf)) { // leaf because i
						    // haven't added
						    // component hierachies
						    // yet
	for (auto component : world->get_all_component_names(entity)) {
	  ImGui::BulletText("%s", component.c_str());
	}
	ImGui::TreePop();
      }
      ImGui::PopID();
    }
    ImGui::TreePop();
  }


  ImGui::End();
}
