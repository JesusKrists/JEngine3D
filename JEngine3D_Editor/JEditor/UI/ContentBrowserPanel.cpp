#include "ContentBrowserPanel.hpp"

#include <JEngine3D/Core/LoggerController.hpp>

#include <imgui.h>
#include <imgui_internal.h>

namespace JEditor {

static constexpr auto CURRENT_DIR_OUTLINE_COLOR = IM_COL32(48, 48, 48, 255);// NOLINT
static constexpr auto CURRENT_DIR_RECT_PADDING = 6;// NOLINT
static constexpr auto BREADCRUMBS_START_OFFSET_X = 48;// NOLINT

static ImFont *s_SmallFont = nullptr;// NOLINT

ContentBrowserPanel::ContentBrowserPanel() : IPanel("Content Browser")
{
  auto &imguiIO = ImGui::GetIO();

  ImFontConfig fontConfigSmall;
  fontConfigSmall.SizePixels = 12;// NOLINT

  // Load default font
  s_SmallFont = imguiIO.Fonts->AddFontDefault(&fontConfigSmall);

  JE::Logger::ClientLogger().info(m_CurrentFolder.c_str());
}

// NOLINTNEXTLINE
void ContentBrowserPanel::OnImGuiRender()
{
  static const ImVec2 TOP_BAR_ICON_SIZE = { ImGui::GetFontSize(), ImGui::GetFontSize() };
  const auto CURRENT_DIR_TRIMMED = TrimCurrentDirToContentDir();

  const ImGuiStyle &IMGUI_STYLE = ImGui::GetCurrentContext()->Style;

  auto RenderBreadCrumbs = [&]() {
    ImGui::PushStyleColor(ImGuiCol_Button, 0);

    auto lastEntryString = CURRENT_DIR_TRIMMED.native().substr(CURRENT_DIR_TRIMMED.native().find_last_of('/') + 1);

    std::filesystem::path currentDepthPath;
    bool firstEntry = true;
    int index = 0;
    for (const auto &entry : CURRENT_DIR_TRIMMED) {
      if (lastEntryString == entry && index == 0) { break; }

      if (!firstEntry) {
        currentDepthPath /= entry;

        ImGui::PushID(++index);

        if (index != 1) { ImGui::SameLine(); }
        if (ImGui::Button(entry.c_str())) { ChangeDirectory(currentDepthPath); }

        if (lastEntryString != entry) {
          ImGui::PushFont(s_SmallFont);
          ImGui::SameLine();
          ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
          ImGui::ArrowButtonEx("##BreadCrumbArrow",
            ImGuiDir_Right,
            ImVec2{ TOP_BAR_ICON_SIZE.x + IMGUI_STYLE.FramePadding.x * 2,
              TOP_BAR_ICON_SIZE.y + IMGUI_STYLE.FramePadding.y * 2 },// NOLINT
            ImGuiItemFlags_Disabled);
          ImGui::PopStyleColor();
          ImGui::PopFont();
        }

        ImGui::PopID();
      }

      firstEntry = false;
    }

    ImGui::PopStyleColor();
  };

  auto RenderTopBar = [&]() {
    ImGui::BeginGroup();
    ImGui::PushStyleColor(ImGuiCol_Button, 0);

    if (ImGui::ArrowButtonEx("##BackArrow", ImGuiDir_Left, TOP_BAR_ICON_SIZE)) {}
    ImGui::SameLine();
    if (ImGui::ArrowButtonEx("##ForwardArrow", ImGuiDir_Right, TOP_BAR_ICON_SIZE)) {}

    ImGui::PopStyleColor();
    ImGui::EndGroup();

    ImGui::SameLine(0, 16);// NOLINT
    ImGui::TextUnformatted("Current Dir:");

    ImGui::SameLine(0, 16);// NOLINT
    ImGui::TextUnformatted(CURRENT_DIR_TRIMMED.c_str());
    auto textRectMin = ImVec2{ ImGui::GetItemRectMin().x - CURRENT_DIR_RECT_PADDING,
      ImGui::GetItemRectMin().y - (CURRENT_DIR_RECT_PADDING - 2) };
    auto textRectMax = ImVec2{ ImGui::GetItemRectMax().x + CURRENT_DIR_RECT_PADDING,
      ImGui::GetItemRectMax().y + CURRENT_DIR_RECT_PADDING };
    ImGui::GetWindowDrawList()->AddRect(textRectMin, textRectMax, CURRENT_DIR_OUTLINE_COLOR);

    ImGui::SameLine(0, BREADCRUMBS_START_OFFSET_X);
    RenderBreadCrumbs();
  };

  RenderTopBar();
}

}// namespace JEditor