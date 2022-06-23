#pragma once

#include <JEngine3D/Core/Application.hpp>
#include <JEngine3D/Core/MemoryController.hpp>
#include "IPanel.hpp"

#include <filesystem>
#include <stack>

namespace JEditor {

struct FileSystemEntry
{
  FileSystemEntry(const std::filesystem::path &path, bool folder) : Path(path), Folder(folder) {}// NOLINT

  std::filesystem::path Path;
  bool Folder;
  bool Subdirectories = false;
  FileSystemEntry *Parent = nullptr;
  JE::Vector<JE::Scope<FileSystemEntry, JE::MemoryTag::Editor>, JE::MemoryTag::Editor> Entries{};
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class ContentBrowserPanel : public IPanel
{
  using NavigationStackContainer = JE::Vector<const FileSystemEntry *, JE::MemoryTag::Editor>;

public:
  const std::filesystem::path CONTENT_HOME_FOLDER = "JEngine3D";
  const std::filesystem::path CONTENT_DIR = "assets";
  const std::filesystem::path CONTENT_FULL_PATH = JE_APP.WORKING_DIRECTORY / CONTENT_DIR;

  ContentBrowserPanel();
  virtual ~ContentBrowserPanel() = default;// NOLINT

private:
  void OnImGuiRender() override;
  void RenderContentTreeEntryRecursive(const FileSystemEntry &folder);
  void RefreshFilesystem();
  void PopulateFolderEntryRecursive(FileSystemEntry &folder);

  [[nodiscard]] inline auto TrimCurrentDirToContentDir() const -> std::filesystem::path
  {
    auto subStringIndex = (*m_CurrentNavigationPosition)->Path.native().find(CONTENT_HOME_FOLDER / CONTENT_DIR);
    return (*m_CurrentNavigationPosition)->Path.native().substr(subStringIndex + CONTENT_HOME_FOLDER.native().length());
  }


  inline void ChangeDirectory(const FileSystemEntry *entry)
  {
    if (*m_CurrentNavigationPosition != entry) {
      m_NavigationStack.erase(std::next(m_CurrentNavigationPosition), std::end(m_NavigationStack));
      m_NavigationStack.push_back(entry);
      m_CurrentNavigationPosition = std::prev(std::end(m_NavigationStack));

      RebuildBreadcrumbsPaths();
    }
  }

  inline void RebuildBreadcrumbsPaths()
  {
    m_BreadcrumbsPaths.clear();
    for (const auto *entry = *m_CurrentNavigationPosition; entry != nullptr; entry = entry->Parent) {
      m_BreadcrumbsPaths.insert(std::begin(m_BreadcrumbsPaths), entry);
    }
  }

  FileSystemEntry m_RootFolder = { CONTENT_FULL_PATH, true };

  JE::Vector<const FileSystemEntry *, JE::MemoryTag::Editor> m_BreadcrumbsPaths;
  NavigationStackContainer m_NavigationStack;
  NavigationStackContainer::iterator m_CurrentNavigationPosition;
};

}// namespace JEditor