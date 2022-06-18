#include "ContentBrowserPanel.hpp"

#include "JEditor/JEditorState.hpp"


#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>
#include <IconsFontAwesome6.h>

namespace JEditor {

static constexpr auto SCROLLBAR_BACKGROUND_COLOR = IM_COL32(0, 0, 0, 32);// NOLINT
// static constexpr auto CURRENT_DIR_OUTLINE_COLOR = IM_COL32(48, 48, 48, 255);// NOLINT
static constexpr auto BREADCRUMBS_START_OFFSET_X = 48;// NOLINT
static constexpr auto FOLDER_TREE_WIDTH = 192;// NOLINT
static constexpr auto FOLDER_CONTENT_ICON_SIZE = 96;
static constexpr auto FOLDER_CONTENT_ICON_HOVER_COLOR = IM_COL32(255, 255, 255, 48);// NOLINT
static constexpr auto FOLDER_CONTENT_ICON_CLICKED_COLOR = IM_COL32(255, 255, 255, 72);// NOLINT

ContentBrowserPanel::ContentBrowserPanel() : IPanel("Content Browser")
{
  auto &imguiIO = ImGui::GetIO();


  // Load default font
  if (EditorState::Get().DefaultFont12 == nullptr) {
    ImFontConfig fontConfigSmall;
    fontConfigSmall.SizePixels = 12;// NOLINT
    EditorState::Get().DefaultFont12 = imguiIO.Fonts->AddFontDefault(&fontConfigSmall);
  }
}

// NOLINTNEXTLINE
void ContentBrowserPanel::OnImGuiRender()
{
  static const ImVec2 TOP_BAR_ICON_SIZE = { ImGui::GetFontSize(), ImGui::GetFontSize() };
  const auto CURRENT_DIR_TRIMMED = TrimCurrentDirToContentDir();

  const ImGuiStyle &IMGUI_STYLE = ImGui::GetCurrentContext()->Style;

  auto RenderBreadCrumbs = [&]() {
    ImGui::PushStyleColor(ImGuiCol_Button, 0);

    const auto lastEntryString =
      CURRENT_DIR_TRIMMED.native().substr(CURRENT_DIR_TRIMMED.native().find_last_of('/') + 1);

    std::filesystem::path currentDepthPath;
    bool firstEntry = true;
    int index = 0;
    for (const auto &entry : CURRENT_DIR_TRIMMED) {
      if (lastEntryString == entry && index == 0) { break; }

      if (!firstEntry) {
        currentDepthPath /= entry;

        ImGui::PushID(++index);

        if (index == 1) {
          ImGui::SameLine(0, BREADCRUMBS_START_OFFSET_X);
        } else {
          ImGui::SameLine(0, 0);
        }
        if (ImGui::Button(entry.c_str())) { ChangeDirectory(currentDepthPath); }

        if (lastEntryString != entry) {
          ImGui::PushFont(EditorState::Get().DefaultFont12);
          ImGui::SameLine(0, 0);
          ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
          ImGui::ArrowButtonEx("##BreadCrumbArrow",
            ImGuiDir_Right,
            ImVec2{ TOP_BAR_ICON_SIZE.x + IMGUI_STYLE.FramePadding.y * 2,
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::BeginChild("TopBar",
      ImVec2{ 0, ImGui::GetFrameHeight() + IMGUI_STYLE.ScrollbarSize },
      false,
      ImGuiWindowFlags_HorizontalScrollbar);


    ImGui::PushStyleColor(ImGuiCol_Button, 0);

    ImGui::SameLine();// NOLINT
    ImGui::SetCursorPosY(IMGUI_STYLE.FramePadding.y);
    if (ImGui::ArrowButtonEx("##BackArrow", ImGuiDir_Left, TOP_BAR_ICON_SIZE, ImGuiItemFlags_Disabled)) {}

    ImGui::SameLine();
    ImGui::SetCursorPosY(IMGUI_STYLE.FramePadding.y);
    if (ImGui::ArrowButtonEx("##ForwardArrow", ImGuiDir_Right, TOP_BAR_ICON_SIZE)) {}

    ImGui::SameLine(0, 8);// NOLINT
    ImGui::SetCursorPosY(IMGUI_STYLE.FramePadding.y);
    ImGui::TextUnformatted("Current Dir:");

    ImGui::SameLine(0, 8);// NOLINT
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
    ImGui::ButtonEx(CURRENT_DIR_TRIMMED.c_str(), ImVec2{ 0, 0 }, ImGuiItemFlags_Disabled);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);

    ImGui::PopStyleColor();

    RenderBreadCrumbs();

    ImGui::EndChild();
    ImGui::PopStyleVar();
  };

  auto RenderFolderTree = [&]() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::BeginChild("FolderTree", ImVec2{ FOLDER_TREE_WIDTH, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNodeEx(CONTENT_DIR.c_str(),
          ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick// NOLINT
            | ImGuiTreeNodeFlags_SpanFullWidth)) {
      if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { ChangeDirectory(CONTENT_DIR); }

      RenderContentTreeEntryRecursive(CONTENT_FULL_PATH);

      ImGui::TreePop();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
  };

  auto RenderFolderContent = [&]() {
    ImGui::BeginChild("FolderContent");

    const auto FOLDER_ICON_TOTAL_WIDTH =
      FOLDER_CONTENT_ICON_SIZE + IMGUI_STYLE.FramePadding.x * 2 + IMGUI_STYLE.ItemSpacing.x;

    auto folderContentRegionSize = ImGui::GetWindowContentRegionMax();
    auto iconsPerRow = std::max(1, static_cast<int>(folderContentRegionSize.x / FOLDER_ICON_TOTAL_WIDTH));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { IMGUI_STYLE.ItemSpacing.x, -8 });// NOLINT
    ImGui::PushStyleColor(ImGuiCol_Button, 0);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);

    int currentIcon = 0;
    int currentIconIndex = 0;
    for (const auto &entry : std::filesystem::directory_iterator{ m_CurrentFolder }) {
      if (currentIcon++ != 0) { ImGui::SameLine(); }

      ImGui::PushID(currentIconIndex++);

      ImGui::BeginChild("FolderContentEntry",
        { FOLDER_CONTENT_ICON_SIZE + IMGUI_STYLE.FramePadding.x * 2,
          FOLDER_CONTENT_ICON_SIZE + ImGui::GetFontSize() + IMGUI_STYLE.ItemSpacing.y
            + IMGUI_STYLE.FramePadding.y * 2 });


      const auto windowWidth = ImGui::GetWindowSize().x;

      auto highlight = false;
      ImU32 hightlightColor = 0;
      if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
        highlight = true;
        hightlightColor = FOLDER_CONTENT_ICON_HOVER_COLOR;
      } else if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && ImGui::IsMouseDown(0)) {
        highlight = true;
        hightlightColor = FOLDER_CONTENT_ICON_CLICKED_COLOR;
      }

      if (highlight) {
        ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(),
          { ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y },
          hightlightColor);
      }

      bool folder = entry.is_directory();
      if (folder) {
        auto folderName = entry.path().stem();
        if (ImGui::ImageButton(EditorState::Get().FileIconMap[FileExtension::FOLDER]->RendererID(),
              ImVec2{ FOLDER_CONTENT_ICON_SIZE, FOLDER_CONTENT_ICON_SIZE })) {
          ChangeDirectory(entry.path());
        }

        const auto textWidth = ImGui::CalcTextSize(folderName.c_str()).x;
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5F);// NOLINT
        ImGui::TextUnformatted(folderName.c_str());
      } else {
        auto fileExtension = entry.path().extension();
        auto fileName = entry.path().stem();
        ImGui::ImageButton(EditorState::Get().FileIconMap[StringToFileExtension(fileExtension)]->RendererID(),
          ImVec2{ FOLDER_CONTENT_ICON_SIZE, FOLDER_CONTENT_ICON_SIZE });

        const auto textWidth = ImGui::CalcTextSize(fileName.c_str()).x;
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5F);// NOLINT
        ImGui::TextUnformatted(fileName.c_str());
      }

      ImGui::EndChild();

      ImGui::PopID();


      if (currentIcon == iconsPerRow) { currentIcon = 0; }
    }

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);

    ImGui::EndChild();
  };

  ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, SCROLLBAR_BACKGROUND_COLOR);

  RenderTopBar();
  RenderFolderTree();
  ImGui::SameLine(0, 4);
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  ImGui::SameLine();
  RenderFolderContent();

  ImGui::PopStyleColor();
}

// NOLINTNEXTLINE
void ContentBrowserPanel::RenderContentTreeEntryRecursive(const std::filesystem::path &path)
{
  for (const auto &entry : std::filesystem::directory_iterator{ path }) {
    if (entry.is_directory()) {
      const auto dirHasSubdirs = DirectoryHasSubdirectories(entry.path());
      const auto entryLabel = entry.path().stem().native();

      if (dirHasSubdirs) {
        bool open = ImGui::TreeNodeEx_IconText(EditorState::Get().FileIconMap[FileExtension::FOLDER]->RendererID(),
          entryLabel.c_str(),
          ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick// NOLINT
            | ImGuiTreeNodeFlags_SpanFullWidth);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { ChangeDirectory(entry.path()); }
        if (open) {
          RenderContentTreeEntryRecursive(entry.path());
          ImGui::TreePop();
        }
      } else {
        ImGui::TreeNodeEx_IconText(EditorState::Get().FileIconMap[FileExtension::FOLDER]->RendererID(),
          entryLabel.c_str(),
          ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen// NOLINT
            | ImGuiTreeNodeFlags_SpanFullWidth);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { ChangeDirectory(entry.path()); }
      }
    }
  }
}

}// namespace JEditor