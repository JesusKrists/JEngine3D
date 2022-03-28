#pragma once

#include "JEngine3D/Platform/IPlatformBackend.hpp"
#include "JEngine3D/Core/Types.hpp"// for Size2D


namespace JE {

class IEventProcessor;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class SDLPlatformBackend final : public IPlatformBackend
{
public:
  ~SDLPlatformBackend() override;

  [[nodiscard]] auto Initialize() -> bool override;
  [[nodiscard]] auto Initialized() -> bool override;

  [[nodiscard]] auto CreateWindow(const std::string_view &title, const Size2D &size) -> NativeWindowHandle override;
  void DestroyWindow(NativeWindowHandle handle) override;

  [[nodiscard]] auto WindowSize(NativeWindowHandle handle) -> Size2D override;
  void SetWindowSize(NativeWindowHandle handle, const Size2D &size) override;

  [[nodiscard]] auto WindowTitle(NativeWindowHandle handle) -> std::string_view override;
  void SetWindowTitle(NativeWindowHandle handle, const std::string_view &title) override;

  void PollEvents(IEventProcessor &processor) override;
};

}// namespace JE