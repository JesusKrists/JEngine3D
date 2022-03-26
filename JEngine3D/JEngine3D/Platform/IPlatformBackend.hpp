#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/Types.hpp"

#include <string_view>

namespace JE {

class EventProcessor;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IPlatformBackend
{
public:
  using NativeWindowHandle = void *;

  IPlatformBackend()
  {
    ASSERT(!s_Instance, "Platform backend instance already created");
    s_Instance = this;
  }

  [[nodiscard]] static inline auto Get() -> IPlatformBackend &
  {
    ASSERT(s_Instance, "Platform backend instance is null");
    return *s_Instance;
  }

  virtual ~IPlatformBackend() { s_Instance = nullptr; };

  [[nodiscard]] virtual auto Initialize() -> bool = 0;

  [[nodiscard]] virtual auto CreateWindow(std::string_view title, const Size2D &size) -> NativeWindowHandle = 0;
  virtual void DestroyWindow(NativeWindowHandle handle) = 0;

  [[nodiscard]] virtual auto WindowSize(NativeWindowHandle handle) -> Size2D = 0;
  virtual void SetWindowSize(NativeWindowHandle handle, const Size2D &size) = 0;

  [[nodiscard]] virtual auto WindowTitle(NativeWindowHandle handle) -> std::string_view = 0;
  virtual void SetWindowTitle(NativeWindowHandle handle, std::string_view title) = 0;

  virtual void PollEvents(EventProcessor &processor) = 0;

private:
  static IPlatformBackend *s_Instance;// NOLINT
};

}// namespace JE