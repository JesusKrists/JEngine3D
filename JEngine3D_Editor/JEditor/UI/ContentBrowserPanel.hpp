#pragma once

#include <JEngine3D/Core/Application.hpp>
#include "IPanel.hpp"

#include <filesystem>

namespace JEditor {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class ContentBrowserPanel : public IPanel
{
public:
  const std::filesystem::path CONTENT_HOME_FOLDER = "JEngine3D";
  const std::filesystem::path CONTENT_DIR = "assets";
  const std::filesystem::path CONTENT_FULL_PATH = JE_APP.WORKING_DIRECTORY / CONTENT_DIR;

  ContentBrowserPanel();
  virtual ~ContentBrowserPanel() = default;// NOLINT

private:
  void OnImGuiRender() override;
  void RenderContentTreeEntryRecursive(const std::filesystem::path &path);

  [[nodiscard]] inline auto TrimCurrentDirToContentDir() const -> std::filesystem::path
  {
    auto subStringIndex = m_CurrentFolder.native().find(CONTENT_HOME_FOLDER / CONTENT_DIR);
    return m_CurrentFolder.native().substr(subStringIndex + CONTENT_HOME_FOLDER.native().length());
  }

  [[nodiscard]] static inline auto DirectoryHasSubdirectories(const std::filesystem::path &path) -> bool
  {
    bool subdirectories = false;
    for (const auto &entry : std::filesystem::directory_iterator{ path }) {
      if (entry.is_directory()) { subdirectories = true; }
    }

    return subdirectories;
  }

  inline void ChangeDirectory(const std::filesystem::path &path) { m_CurrentFolder = JE_APP.WORKING_DIRECTORY / path; }


  std::filesystem::path m_CurrentFolder = CONTENT_FULL_PATH / "textures";
};

}// namespace JEditor