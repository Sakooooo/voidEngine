#include "camera.h"
#include "guis.h"
#include "imgui.h"
#include "player_controller.h"
#include "rectangle.h"
#include "script.h"
#include "world.h"
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_stdinc.h>
#include <cfloat>
#include <cstdio>
#include <functional>
#include <string_view>
#include <vector>

/// Draws the fields of a component inside its tree node
using ComponentInspector =
    std::function<void(World::CWorld* world, Entity entity)>;

static void inspect_transform2d(World::CWorld* world, Entity entity) {
  auto& transform = world->get_component<Transform2D>(entity);
  ImGui::DragFloat2("pos", &transform.pos.x);
  ImGui::DragFloat("width", &transform.width, 1.0f, 0.0f, FLT_MAX);
  ImGui::DragFloat("height", &transform.height, 1.0f, 0.0f, FLT_MAX);
}

static void inspect_color(World::CWorld* world, Entity entity) {
  auto& color = world->get_component<Color>(entity);
  // Color stores ints from 0-255, ImGui wants floats from 0-1
  float rgba[4] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
                   color.a / 255.0f};
  if (ImGui::ColorEdit4("rgba", rgba)) {
    color.r = static_cast<int>(rgba[0] * 255.0f + 0.5f);
    color.g = static_cast<int>(rgba[1] * 255.0f + 0.5f);
    color.b = static_cast<int>(rgba[2] * 255.0f + 0.5f);
    color.a = static_cast<int>(rgba[3] * 255.0f + 0.5f);
  }
}

static void inspect_camera2d(World::CWorld* world, Entity entity) {
  auto& camera = world->get_component<Camera2D>(entity);
  ImGui::Checkbox("current", &camera.current);
  ImGui::DragFloat("zoom", &camera.zoom, 0.01f, 0.01f, 100.0f);
  if (camera.follow) {
    ImGui::Text("follow: %u (gen %u)%s", camera.follow->entity,
                camera.follow->generation,
                world->is_valid(*camera.follow) ? "" : " [invalid]");
  } else {
    ImGui::TextDisabled("follow: none");
  }
}

static void inspect_player_controller(World::CWorld* world, Entity entity) {
  auto& controller = world->get_component<PlayerController>(entity);
  // these are driven by input, so just display them
  ImGui::BeginDisabled();
  ImGui::Checkbox("up", &controller.up);
  ImGui::SameLine();
  ImGui::Checkbox("down", &controller.down);
  ImGui::SameLine();
  ImGui::Checkbox("left", &controller.left);
  ImGui::SameLine();
  ImGui::Checkbox("right", &controller.right);
  ImGui::EndDisabled();
}

/// Only allow characters that are valid in a plain filename, no directories
static int filename_char_filter(ImGuiInputTextCallbackData* data) {
  return data->EventChar == '/' || data->EventChar == '\\';
}

static void inspect_luau_script(World::CWorld* world, Entity entity) {
  auto& script = world->get_component<LuauScript>(entity);
  // the script system reloads whenever the path changes, so only commit the
  // new path when enter is pressed instead of on every keystroke
  char buffer[256];
  std::snprintf(buffer, sizeof(buffer), "%s", script.path.c_str());
  if (ImGui::InputText("path", buffer, sizeof(buffer),
                       ImGuiInputTextFlags_EnterReturnsTrue |
                           ImGuiInputTextFlags_CallbackCharFilter,
                       filename_char_filter)) {
    script.path = buffer;
  }
  ImGui::SetItemTooltip("Filename inside the scripts folder, enter to apply");

  // pick from the scripts that actually exist, only globbed while open
  if (ImGui::BeginCombo("scripts", script.path.c_str())) {
    const char* base_path = SDL_GetBasePath();
    std::string scripts_path =
        std::string(base_path ? base_path : "") + "scripts";
    int count = 0;
    char** files = SDL_GlobDirectory(scripts_path.c_str(), "*.luau", 0, &count);
    for (int i = 0; i < count; ++i) {
      if (ImGui::Selectable(files[i], script.path == files[i]))
        script.path = files[i];
    }
    if (count == 0)
      ImGui::TextDisabled("No .luau files in %s", scripts_path.c_str());
    SDL_free(files);
    ImGui::EndCombo();
  }

  if (ImGui::Button("Reload"))
    script.reload = true;
}

/// Everything the tree needs to know about a component type
struct ComponentEntry {
  std::string_view name;
  /// nullptr if there's nothing to show, the component is drawn as a leaf
  ComponentInspector inspect;
  std::function<bool(World::CWorld* world, Entity entity)> has;
  /// Add the component with its default value
  std::function<void(World::CWorld* world, Entity entity)> add;
  std::function<void(World::CWorld* world, Entity entity)> remove;
  /// Copy the component from one entity onto another
  std::function<void(World::CWorld* world, Entity from, Entity to)> copy;
};

template <ComponentConcept ComponentType>
static ComponentEntry make_entry(ComponentInspector inspect,
                                 ComponentType default_value = {}) {
  return {
      .name = ComponentType::name,
      .inspect = std::move(inspect),
      .has =
          [](World::CWorld* world, Entity entity) {
            return world->has_component<ComponentType>(entity);
          },
      .add =
          [default_value](World::CWorld* world, Entity entity) {
            world->add_component<ComponentType>(entity, default_value);
          },
      .remove =
          [](World::CWorld* world, Entity entity) {
            world->remove_component<ComponentType>(entity);
          },
      .copy =
          [](World::CWorld* world, Entity from, Entity to) {
            world->add_component<ComponentType>(
                to, world->get_component<ComponentType>(from));
          },
  };
}

/// Every component the tree knows about, add new components here to be able to
/// inspect, add, remove and duplicate them
static const std::vector<ComponentEntry> components = {
    make_entry<Transform2D>(
        inspect_transform2d,
        {.pos = {0.0f, 0.0f}, .width = 100.0f, .height = 100.0f}),
    make_entry<Color>(inspect_color, {.r = 255, .g = 255, .b = 255, .a = 255}),
    make_entry<Rect>(nullptr),
    make_entry<Camera2D>(inspect_camera2d),
    make_entry<PlayerController>(inspect_player_controller),
    make_entry<LuauScript>(inspect_luau_script),
};

static const ComponentEntry* find_component(std::string_view name) {
  for (auto& entry : components) {
    if (entry.name == name)
      return &entry;
  }
  return nullptr;
}

/// Changes to the world that are applied after the tree is drawn, so nothing
/// gets modified while all_entities() is being iterated or a component is
/// being inspected
using DeferredActions = std::vector<std::function<void()>>;

/// Menu items for every component the entity doesn't have yet
static void add_component_items(World::CWorld* world, Entity entity,
                                DeferredActions& deferred) {
  bool any = false;
  for (auto& entry : components) {
    if (entry.has(world, entity))
      continue;
    any = true;
    if (ImGui::MenuItem(entry.name.data()))
      deferred.push_back([&entry, world, entity] { entry.add(world, entity); });
  }
  if (!any)
    ImGui::TextDisabled("No components left to add");
}

/// Create a new entity with copies of every known component of entity
static void duplicate_entity(World::CWorld* world, Entity entity) {
  Entity copy = world->create_entity();
  for (auto& entry : components) {
    if (entry.has(world, entity))
      entry.copy(world, entity, copy);
  }
}

/// Check if the entity's label or any of its components match the filter
static bool passes_filter(const ImGuiTextFilter& filter, const char* label,
                          const std::vector<std::string>& entity_components) {
  if (!filter.IsActive() || filter.PassFilter(label))
    return true;
  for (auto& component : entity_components) {
    if (filter.PassFilter(component.c_str()))
      return true;
  }
  return false;
}

static void render_component(World::CWorld* world, Entity entity,
                             const std::string& component,
                             DeferredActions& deferred) {
  ImGui::PushID(component.c_str());
  auto entry = find_component(component);

  bool open;
  if (!entry || !entry->inspect) {
    ImGui::TreeNodeEx(component.c_str(),
                      ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet |
                          ImGuiTreeNodeFlags_NoTreePushOnOpen);
    open = false;
  } else {
    open = ImGui::TreeNodeEx(component.c_str());
  }

  // context menu has to come right after the tree node so it attaches to it
  if (ImGui::BeginPopupContextItem("component_context")) {
    if (!entry) {
      ImGui::TextDisabled("Unknown component, add it to the tree registry");
    } else if (ImGui::MenuItem("Remove Component")) {
      deferred.push_back(
          [entry, world, entity] { entry->remove(world, entity); });
    }
    ImGui::EndPopup();
  }

  if (open) {
    entry->inspect(world, entity);
    ImGui::TreePop();
  }

  ImGui::PopID();
}

static void render_entity(World::CWorld* world, Entity entity,
                          const ImGuiTextFilter& filter,
                          DeferredActions& deferred) {
  auto entity_components = world->get_all_component_names(entity);
  char label[32];
  std::snprintf(label, sizeof(label), "Entity %u", entity);
  if (!passes_filter(filter, label, entity_components))
    return;

  ImGui::PushID(entity);

  ImGuiTreeNodeFlags entity_flags =
      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
  bool open = ImGui::TreeNodeEx("entity", entity_flags, "%s", label);
  if (!entity_components.empty()) {
    ImGui::SameLine();
    ImGui::TextDisabled("(%zu)", entity_components.size());
  }

  if (ImGui::BeginPopupContextItem("entity_context")) {
    if (ImGui::BeginMenu("Add Component")) {
      add_component_items(world, entity, deferred);
      ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Duplicate Entity"))
      deferred.push_back([world, entity] { duplicate_entity(world, entity); });
    ImGui::Separator();
    if (ImGui::MenuItem("Destroy Entity"))
      deferred.push_back([world, entity] { world->destroy_entity(entity); });
    ImGui::EndPopup();
  }

  if (open) {
    for (auto& component : entity_components)
      render_component(world, entity, component, deferred);

    if (ImGui::SmallButton("+ Add Component"))
      ImGui::OpenPopup("add_component");
    if (ImGui::BeginPopup("add_component")) {
      add_component_items(world, entity, deferred);
      ImGui::EndPopup();
    }

    ImGui::TreePop();
  }

  ImGui::PopID();
}

void TreeGui::render(Engine::CEngine* engine, World::CWorld* world) {
  static ImGuiTextFilter filter;
  DeferredActions deferred;

  ImGui::Begin("Tree");
  ImGui::Text("Entities: %zu", world->all_entities().size());
  ImGui::SameLine();
  if (ImGui::Button("Create Entity"))
    deferred.push_back([world] { world->create_entity(); });
  filter.Draw("Filter");

  if (ImGui::TreeNodeEx("Entities", ImGuiTreeNodeFlags_DefaultOpen)) {
    for (auto entity : world->all_entities())
      render_entity(world, entity, filter, deferred);
    ImGui::TreePop();
  }

  ImGui::End();

  for (auto& action : deferred)
    action();
}
