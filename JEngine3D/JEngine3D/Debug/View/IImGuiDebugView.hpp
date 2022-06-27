#pragma once

#include <string>

#include <imgui.h>

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IImGuiDebugView
{
public:
  static constexpr auto PARAMETER_COLOR = Color{ 1, 1, 0, 1 };
  static constexpr auto TRUE_COLOR = Color{ 0, 1, 0, 1 };
  static constexpr auto FALSE_COLOR = Color{ 1, 0, 0, 1 };

  explicit IImGuiDebugView(const std::string_view &name) : m_Name(name) {}
  virtual ~IImGuiDebugView() = default;

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
  bool m_Open = false;
};

}// namespace JE