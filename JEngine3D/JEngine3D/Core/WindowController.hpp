#pragma once

#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Platform/IPlatformBackend.hpp"

#include <string_view>
#include <string>// for string

namespace JE {


class Window
{
  friend class WindowController;

public:
  Window(const Window &other) = delete;
  auto operator=(const Window &other) -> Window & = delete;
  Window(Window &&other) = delete;
  auto operator=(Window &&other) -> Window & = delete;

  Window(const std::string_view &title, const Size2D &size, IPlatformBackend::NativeWindowHandle nativeHandle);
  ~Window();

  [[nodiscard]] inline auto GetNativeHandle() const -> IPlatformBackend::NativeWindowHandle { return m_NativeHandle; }

  [[nodiscard]] inline auto Title() const -> const std::string & { return m_Title; }
  void SetTitle(const std::string_view &title);

  [[nodiscard]] inline auto Size() const -> const Size2D & { return m_Size; }
  void SetSize(const Size2D &size);

private:
  IPlatformBackend::NativeWindowHandle m_NativeHandle;
  std::string m_Title;
  Size2D m_Size;
};

class WindowController
{
  using WindowContainer = Vector<Scope<Window, MemoryTag::App>, MemoryTag::App>;

public:
  WindowController();

  auto CreateWindow(const std::string_view &title, const Size2D &size) -> Window &;
  [[nodiscard]] inline auto Windows() const -> const WindowContainer & { return m_Windows; }

  inline void DeleteAllWindows() { m_Windows.clear(); }

private:
  WindowContainer m_Windows;
};

}// namespace JE