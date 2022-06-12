#pragma once

#include <string>
#include <imgui.h>

namespace JEditor {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IPanel
{
public:
  explicit IPanel(const std::string_view &name) : m_Name(name) {}
  virtual ~IPanel() = default;

  void Render()
  {
    if (ImGui::Begin(Name().c_str(), &m_Open)) { OnImGuiRender(); }
    ImGui::End();
  }

  [[nodiscard]] inline auto Name() const -> const std::string & { return m_Name; }

  inline void Open() { m_Open = true; };
  inline void Close() { m_Open = false; };
  [[nodiscard]] inline auto IsOpen() const -> bool { return m_Open; }

private:
  virtual void OnImGuiRender() = 0;

  const std::string m_Name;
  bool m_Open = true;
};

}// namespace JEditor