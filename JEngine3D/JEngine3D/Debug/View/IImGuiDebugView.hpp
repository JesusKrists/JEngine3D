#pragma once

#include <string>

#include <imgui.h>

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IImGuiDebugView
{
public:
  static constexpr auto PARAMETER_COLOR = ImVec4{ 1, 1, 0, 1 };
  static constexpr auto TRUE_COLOR = ImVec4{ 0, 1, 0, 1 };
  static constexpr auto FALSE_COLOR = ImVec4{ 1, 0, 0, 1 };

  explicit IImGuiDebugView(const std::string_view &name) : m_Name(name) {}
  virtual ~IImGuiDebugView() = default;


  virtual void OnImGuiRender() = 0;


  [[nodiscard]] inline auto Name() const -> const std::string & { return m_Name; }

  inline void Open() { m_Open = true; };
  inline void Close() { m_Open = false; };
  [[nodiscard]] inline auto IsOpen() const -> bool { return m_Open; }

protected:
  bool m_Open = false;

private:
  const std::string m_Name;
};

}// namespace JE