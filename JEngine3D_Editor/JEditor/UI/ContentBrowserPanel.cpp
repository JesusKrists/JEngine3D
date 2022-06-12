#include "ContentBrowserPanel.hpp"
#include <imgui.h>

namespace JEditor {

void ContentBrowserPanel::OnImGuiRender()
{
  auto RenderBreadCrumbs = [&]() {
    ImGui::BeginChild("Breadcrumbs", ImVec2{ 0, 0 }, true, ImGuiWindowFlags_AlwaysAutoResize);

    if (m_CurrentFolder.has_parent_path()) {
      if (ImGui::Selectable(m_CurrentFolder.c_str())) {}
    } else {
      if (ImGui::Selectable("/", false)) { m_CurrentFolder = ROOT_CONTENT_DIR; }
    }

    ImGui::EndChild();
  };

  auto RenderContentTree = [&]() {
    static constexpr ImVec2 CONTENT_TREE_SIZE = { 150, 0 };

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick// NOLINT
                               | ImGuiTreeNodeFlags_SpanAvailWidth;// NOLINT

    ImGuiTreeNodeFlags rootNodeFlags = flags;
    if (m_CurrentFolder == ROOT_CONTENT_DIR) { rootNodeFlags |= ImGuiTreeNodeFlags_Selected; }// NOLINT

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4);
    ImGui::BeginChild("ContentTree", CONTENT_TREE_SIZE, true);

    auto rootNodeOpen = ImGui::TreeNodeEx(ROOT_CONTENT_DIR.c_str(), rootNodeFlags);
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { m_CurrentFolder = ROOT_CONTENT_DIR; }


    if (rootNodeOpen) { ImGui::TreePop(); }

    ImGui::EndChild();
    ImGui::PopStyleVar(1);
  };

  RenderBreadCrumbs();
  RenderContentTree();
  ImGui::SameLine();
  ImGui::BeginGroup();
  for (const auto &entry : std::filesystem::directory_iterator(ROOT_CONTENT_FULL_PATH)) {
    ImGui::TextUnformatted(entry.path().filename().c_str());
  }
  ImGui::EndGroup();
}

}// namespace JEditor