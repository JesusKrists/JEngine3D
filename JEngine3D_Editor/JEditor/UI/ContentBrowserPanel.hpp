#pragma once

#include "IPanel.hpp"

#include <filesystem>

namespace JEditor {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class ContentBrowserPanel : public IPanel
{
public:
  const std::filesystem::path ROOT_CONTENT_DIR = "assets";
  const std::filesystem::path ROOT_CONTENT_FULL_PATH = std::filesystem::current_path() / ROOT_CONTENT_DIR;

  ContentBrowserPanel() : IPanel("Content Browser") {}
  virtual ~ContentBrowserPanel() = default;// NOLINT

private:
  void OnImGuiRender() override;


  std::filesystem::path m_CurrentFolder = ROOT_CONTENT_DIR;
};

}// namespace JEditor