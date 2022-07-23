#include "ContentBrowserPanel.hpp"

#include "JEditor/JEditorState.hpp"


#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>
#include <IconsFontAwesome6.h>

namespace JEditor {

    static constexpr auto DISABLED_TEXT_COLOR        = IM_COL32(255, 255, 255, 48);// NOLINT
    static constexpr auto SCROLLBAR_BACKGROUND_COLOR = IM_COL32(0, 0, 0, 32);// NOLINT

    static constexpr auto BREADCRUMBS_START_OFFSET_X = 48;// NOLINT

    static constexpr auto FOLDER_TREE_WIDTH = 192;// NOLINT

    static constexpr auto FOLDER_CONTENT_ICON_SIZE            = 96;
    static constexpr auto FOLDER_CONTENT_ICON_HOVER_COLOR     = IM_COL32(255, 255, 255, 48);// NOLINT
    static constexpr auto FOLDER_CONTENT_ICON_CLICKED_COLOR   = IM_COL32(255, 255, 255, 72);// NOLINT
    static constexpr auto FOLDER_CONTENT_EXTENSION_BACKGROUND = IM_COL32(96, 96, 96, 192);// NOLINT

    ContentBrowserPanel::ContentBrowserPanel() : IPanel("Content Browser")
    {
        static constexpr std::array<ImWchar, 2> icon_ranges = { ICON_MIN_FA, ICON_MAX_FA };// NOLINT
        JE_APP.ImGuiLayer().PushMergeFont("assets/EditorUI/fonts/" FONT_ICON_FILE_NAME_FAS,
                                          13.0F,// NOLINT
                                          13.0F,// NOLINT
                                          icon_ranges);// NOLINT
        JE_APP.ImGuiLayer().PushMergeFont("assets/EditorUI/fonts/" FONT_ICON_FILE_NAME_FAR,
                                          13.0F,// NOLINT
                                          13.0F,// NOLINT
                                          icon_ranges,
                                          true);// NOLINT

        EditorState::Get().DefaultFont = JE_APP.ImGuiLayer().AddImGuiInternalFont();

        JE_APP.ImGuiLayer().PopMergeFont();
        JE_APP.ImGuiLayer().PopMergeFont();

        // Load default font
        ImFontConfig fontConfigSmall;
        fontConfigSmall.SizePixels       = 12;// NOLINT
        EditorState::Get().DefaultFont12 = JE_APP.ImGuiLayer().AddImGuiInternalFont(&fontConfigSmall);

        m_BreadcrumbsPaths.reserve(32);// NOLINT
        RefreshFilesystem();
    }

    // NOLINTNEXTLINE
    void ContentBrowserPanel::OnImGuiRender()
    {
        const ImVec2 TOP_BAR_ICON_SIZE   = { ImGui::GetFontSize(), ImGui::GetFontSize() };
        const auto   CURRENT_DIR_TRIMMED = TrimCurrentDirToContentDir();

        const ImGuiStyle& IMGUI_STYLE = ImGui::GetCurrentContext()->Style;

        auto RenderBreadCrumbs = [&]() {
            ImGui::PushStyleColor(ImGuiCol_Button, 0);


            if (m_BreadcrumbsPaths.size() > 1) {
                int index = 0;
                for (const auto& entry : m_BreadcrumbsPaths) {
                    const auto FOLDER_NAME = entry->Path.stem().generic_string();

                    ImGui::PushID(index);
                    if (index++ == 0) {
                        ImGui::SameLine(0, BREADCRUMBS_START_OFFSET_X);
                    } else {
                        ImGui::SameLine(0, 0);
                    }
                    if (ImGui::Button(FOLDER_NAME.c_str())) { ChangeDirectory(entry); }

                    if (entry != m_BreadcrumbsPaths.back()) {
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
            }

            ImGui::PopStyleColor();
        };

        auto RenderTopBar = [&]() {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::BeginChild(
            "TopBar", ImVec2{ 0, ImGui::GetFrameHeight() + IMGUI_STYLE.ScrollbarSize }, false, ImGuiWindowFlags_HorizontalScrollbar);


            ImGui::PushStyleColor(ImGuiCol_Button, 0);

            bool backwardsDisabled = m_CurrentNavigationPosition == std::begin(m_NavigationStack);
            bool forwardsDisabled  = m_CurrentNavigationPosition == std::prev(std::end(m_NavigationStack));

            if (backwardsDisabled) {
                ImGui::PushStyleColor(ImGuiCol_Text, DISABLED_TEXT_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
            }
            ImGui::SameLine();// NOLINT
            ImGui::SetCursorPosY(IMGUI_STYLE.FramePadding.y);
            if (ImGui::ArrowButtonEx("##BackArrow", ImGuiDir_Left, TOP_BAR_ICON_SIZE, backwardsDisabled ? ImGuiItemFlags_Disabled : 0)) {
                --m_CurrentNavigationPosition;
                RebuildBreadcrumbsPaths();
            }
            if (backwardsDisabled) { ImGui::PopStyleColor(2); }


            if (forwardsDisabled) {
                ImGui::PushStyleColor(ImGuiCol_Text, DISABLED_TEXT_COLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(IMGUI_STYLE.FramePadding.y);
            if (ImGui::ArrowButtonEx("##ForwardArrow", ImGuiDir_Right, TOP_BAR_ICON_SIZE, forwardsDisabled ? ImGuiItemFlags_Disabled : 0)) {
                ++m_CurrentNavigationPosition;
                RebuildBreadcrumbsPaths();
            }
            if (forwardsDisabled) { ImGui::PopStyleColor(2); }

            ImGui::SameLine();
            ImGui::SetCursorPosY(IMGUI_STYLE.FramePadding.y);
            ImGui::PushFont(EditorState::Get().DefaultFont);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 2, 3 });
            if (ImGui::Button(ICON_FA_ARROWS_ROTATE, TOP_BAR_ICON_SIZE)) { RefreshFilesystem(); }
            ImGui::PopStyleVar();
            ImGui::PopFont();

            ImGui::SameLine(0, 24);// NOLINT
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

            int treeFlags =
            ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;// NOLINT
            treeFlags |= (*m_CurrentNavigationPosition == &m_RootFolder) ? ImGuiTreeNodeFlags_Selected : 0;// NOLINT

            ImGui::SetNextItemOpen(true);
            if (ImGui::TreeNodeEx_IconText(EditorState::Get().FileIconMap[FileExtension::FOLDER]->RendererID(),
                                           EditorState::Get().FileIconMap[FileExtension::FOLDER_OPEN]->RendererID(),
                                           CONTENT_DIR.generic_string().c_str(),
                                           treeFlags)) {
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { ChangeDirectory(&m_RootFolder); }

                RenderContentTreeEntryRecursive(m_RootFolder);

                ImGui::TreePop();
            }

            ImGui::EndChild();
            ImGui::PopStyleVar();
        };

        auto RenderFolderContent = [&]() {
            ImGui::BeginChild("FolderContent");

            const auto FOLDER_ICON_TOTAL_WIDTH = FOLDER_CONTENT_ICON_SIZE + IMGUI_STYLE.FramePadding.x * 2 + IMGUI_STYLE.ItemSpacing.x;
            const auto FOLDER_ICON_SIZE        = ImVec2{ FOLDER_CONTENT_ICON_SIZE, FOLDER_CONTENT_ICON_SIZE };
            const auto EXTENSION_TEXT_OFFSET   = ImVec2{ -12, -6 };

            auto folderContentRegionWidth = ImGui::GetWindowWidth() + IMGUI_STYLE.ScrollbarSize;
            auto iconsPerRow              = std::max(1, static_cast<int>(folderContentRegionWidth / FOLDER_ICON_TOTAL_WIDTH));

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
            ImGui::PushStyleColor(ImGuiCol_Button, 0);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);

            int currentIcon      = 0;
            int currentIconIndex = 0;
            for (const auto& entry : (*m_CurrentNavigationPosition)->Entries) {
                if (currentIcon++ != 0) { ImGui::SameLine(); }

                ImGui::PushID(currentIconIndex++);

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { IMGUI_STYLE.ItemSpacing.x, -8 });// NOLINT
                ImGui::BeginChild(
                "FolderContentEntry",
                { FOLDER_CONTENT_ICON_SIZE + IMGUI_STYLE.FramePadding.x * 2,
                  FOLDER_CONTENT_ICON_SIZE + ImGui::GetFontSize() + IMGUI_STYLE.ItemSpacing.y + IMGUI_STYLE.FramePadding.y * 2 });


                const auto windowWidth = ImGui::GetWindowSize().x;

                auto  highlight       = false;
                ImU32 hightlightColor = 0;
                if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
                    highlight       = true;
                    hightlightColor = FOLDER_CONTENT_ICON_HOVER_COLOR;
                } else if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && ImGui::IsMouseDown(0)) {
                    highlight       = true;
                    hightlightColor = FOLDER_CONTENT_ICON_CLICKED_COLOR;
                }

                if (highlight) {
                    ImGui::GetWindowDrawList()->AddRectFilled(
                    ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), hightlightColor);
                }

                bool folder = entry->Folder;
                if (folder) {
                    const auto FOLDER_NAME = entry->Path.stem().generic_string();
                    if (ImGui::ImageButton(EditorState::Get().FileIconMap[FileExtension::FOLDER]->RendererID(), FOLDER_ICON_SIZE)) {
                        ChangeDirectory(entry.get());
                    }

                    const auto textWidth = ImGui::CalcTextSize(FOLDER_NAME.c_str()).x;
                    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5F);// NOLINT
                    ImGui::TextUnformatted(FOLDER_NAME.c_str());
                } else {
                    ImGui::BeginGroup();
                    const auto FILE_EXTENSION       = entry->Path.extension().generic_string();
                    const auto FILE_EXTENSION_UPPER = JE::ToUpper(FILE_EXTENSION);
                    const auto FILE_NAME            = entry->Path.stem().generic_string();
                    ImGui::ImageButton(EditorState::Get().FileIconMap[StringToFileExtension(FILE_EXTENSION)]->RendererID(),
                                       FOLDER_ICON_SIZE);

                    ImGui::PushFont(EditorState::Get().Segoe24Bold);
                    const auto extensionTextSize = ImGui::CalcTextSize(FILE_EXTENSION_UPPER.c_str() + 1);// NOLINT Skip the .
                    const auto textPos           = ImGui::GetWindowPos() - extensionTextSize + FOLDER_ICON_SIZE + EXTENSION_TEXT_OFFSET;
                    ImGui::GetWindowDrawList()->AddRectFilled(
                    textPos - ImVec2{ IMGUI_STYLE.FramePadding.x, IMGUI_STYLE.FramePadding.y - 4 },
                    textPos + extensionTextSize + ImVec2{ IMGUI_STYLE.FramePadding.x, IMGUI_STYLE.FramePadding.y - 2 },
                    FOLDER_CONTENT_EXTENSION_BACKGROUND,
                    2);
                    ImGui::GetWindowDrawList()->AddText(
                    textPos, ImGui::GetColorU32(ImGuiCol_Text), FILE_EXTENSION_UPPER.c_str() + 1);// NOLINT Skip the .
                    ImGui::PopFont();

                    const auto textWidth = ImGui::CalcTextSize(FILE_NAME.c_str()).x;
                    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5F);// NOLINT
                    ImGui::TextUnformatted(FILE_NAME.c_str());
                    ImGui::EndGroup();

                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                        const auto FULL_PATH = entry->Path.generic_string();
                        // Set payload to carry the index of our item (could be anything)
                        ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", FULL_PATH.c_str(), FULL_PATH.size());

                        ImGui::Text("%s%s", FILE_NAME.c_str(), FILE_EXTENSION.c_str());// NOLINT

                        ImGui::EndDragDropSource();
                    }
                }

                ImGui::PopStyleVar();
                ImGui::EndChild();

                ImGui::PopID();


                if (currentIcon == iconsPerRow) { currentIcon = 0; }
            }

            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();

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
    void ContentBrowserPanel::RenderContentTreeEntryRecursive(const FileSystemEntry& folder)
    {
        for (const auto& entry : folder.Entries) {
            if (entry->Folder) {
                const auto FOLDER_NAME = entry->Path.stem().generic_string();

                int treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick// NOLINT
                                | ImGuiTreeNodeFlags_SpanFullWidth;
                treeFlags |= (*m_CurrentNavigationPosition == entry.get()) ? ImGuiTreeNodeFlags_Selected : 0;// NOLINT
                treeFlags |= !entry->Subdirectories ? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen : 0;// NOLINT

                bool open = ImGui::TreeNodeEx_IconText(EditorState::Get().FileIconMap[FileExtension::FOLDER]->RendererID(),
                                                       EditorState::Get().FileIconMap[FileExtension::FOLDER_OPEN]->RendererID(),
                                                       FOLDER_NAME.c_str(),
                                                       treeFlags);
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { ChangeDirectory(entry.get()); }
                if (open && entry->Subdirectories) {
                    RenderContentTreeEntryRecursive(*entry);
                    ImGui::TreePop();
                }
            }
        }
    }

    void ContentBrowserPanel::RefreshFilesystem()
    {
        m_RootFolder.Entries.clear();
        PopulateFolderEntryRecursive(m_RootFolder);

        m_NavigationStack.clear();
        m_NavigationStack.push_back(&m_RootFolder);
        m_CurrentNavigationPosition = std::begin(m_NavigationStack);

        RebuildBreadcrumbsPaths();
    }

    void ContentBrowserPanel::PopulateFolderEntryRecursive(FileSystemEntry& folder)// NOLINT
    {
        for (const auto& entry : std::filesystem::directory_iterator{ folder.Path }) {
            folder.Entries.push_back(JE::CreateScope<FileSystemEntry, JE::MemoryTag::Editor>(entry.path(), entry.is_directory()));
            if (entry.is_directory()) {
                folder.Subdirectories = true;

                const auto& newEntry = folder.Entries.back();
                newEntry->Parent     = &folder;
                PopulateFolderEntryRecursive(*newEntry);
            }
        }
    }

}// namespace JEditor