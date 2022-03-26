#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/Types.hpp"

#include <string_view>

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IPlatformBackend
{
public:
  using NativeWindowHandle = void *;

  IPlatformBackend()
  {
    ASSERT(!Instance, "Platform backend instance already created");
    Instance = this;
  }

  [[nodiscard]] static inline auto Get() -> IPlatformBackend &
  {
    ASSERT(Instance, "Platform backend instance is null");
    return *Instance;
  }

  virtual ~IPlatformBackend() { Instance = nullptr; };

  [[nodiscard]] virtual auto Initialize() -> bool = 0;

  [[nodiscard]] virtual auto CreateWindow(std::string_view title, const Size2D &size) -> NativeWindowHandle = 0;
  virtual void DestroyWindow(NativeWindowHandle handle) = 0;

  [[nodiscard]] virtual auto WindowSize(NativeWindowHandle handle) -> Size2D = 0;
  virtual void SetWindowSize(NativeWindowHandle handle, const Size2D &size) = 0;

  [[nodiscard]] virtual auto WindowTitle(NativeWindowHandle handle) -> std::string_view = 0;
  virtual void SetWindowTitle(NativeWindowHandle handle, std::string_view title) = 0;

private:
  static IPlatformBackend *Instance;// NOLINT
};

}// namespace JE