#pragma once

#include "JEngine3D/Platform/IPlatformBackend.hpp"
#include "JEngine3D/Core/Types.hpp"// for Size2D

#include <string_view>// for string_view

namespace JE {

class EventProcessor;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class SDLPlatformBackend final : public IPlatformBackend
{
public:
  ~SDLPlatformBackend() override;

  [[nodiscard]] auto Initialize() -> bool override;

  [[nodiscard]] auto CreateWindow(std::string_view title, const Size2D &size) -> NativeWindowHandle override;
  void DestroyWindow(NativeWindowHandle handle) override;

  [[nodiscard]] auto WindowSize(NativeWindowHandle handle) -> Size2D override;
  void SetWindowSize(NativeWindowHandle handle, const Size2D &size) override;

  [[nodiscard]] auto WindowTitle(NativeWindowHandle handle) -> std::string_view override;
  void SetWindowTitle(NativeWindowHandle handle, std::string_view title) override;

  void PollEvents(EventProcessor &processor) override;
};

}// namespace JE