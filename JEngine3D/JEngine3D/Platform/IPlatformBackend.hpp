#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/Types.hpp"

namespace JE {

class IEventProcessor;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IPlatformBackend
{
public:
  using NativeWindowHandle = void *;

  IPlatformBackend()
  {
    ASSERT(!s_PlatformBackendInstance, "Platform backend instance already created");
    s_PlatformBackendInstance = this;
  }

  [[nodiscard]] static inline auto Get() -> IPlatformBackend &
  {
    ASSERT(s_PlatformBackendInstance, "Platform backend instance is null");
    return *s_PlatformBackendInstance;
  }

  virtual ~IPlatformBackend() { s_PlatformBackendInstance = nullptr; };

  [[nodiscard]] virtual auto Initialize() -> bool = 0;
  [[nodiscard]] virtual auto Initialized() -> bool = 0;

  [[nodiscard]] virtual auto CreateWindow(const std::string_view &title, const Size2D &size) -> NativeWindowHandle = 0;
  virtual void DestroyWindow(NativeWindowHandle handle) = 0;

  [[nodiscard]] virtual auto WindowSize(NativeWindowHandle handle) -> Size2D = 0;
  virtual void SetWindowSize(NativeWindowHandle handle, const Size2D &size) = 0;

  [[nodiscard]] virtual auto WindowTitle(NativeWindowHandle handle) -> std::string_view = 0;
  virtual void SetWindowTitle(NativeWindowHandle handle, const std::string_view &title) = 0;

  virtual void PollEvents(IEventProcessor &processor) = 0;

private:
  static IPlatformBackend *s_PlatformBackendInstance;// NOLINT
};

}// namespace JE