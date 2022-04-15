#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Platform/IPlatformBackend.hpp"
#include "JEngine3D/Platform/IGraphicsContextCreator.hpp"
#include "JEngine3D/Core/Types.hpp"

namespace JE {

class Window;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IGraphicsContext
{
public:
  IGraphicsContext(IPlatformBackend::NativeWindowHandle windowHandle,// NOLINT(bugprone-easily-swappable-parameters)
    IGraphicsContextCreator::NativeContextHandle contextHandle)
    : m_WindowHandle(windowHandle), m_ContextHandle(contextHandle)
  {}
  [[nodiscard]] auto inline NativeWindowHandle() -> IPlatformBackend::NativeWindowHandle { return m_WindowHandle; }
  [[nodiscard]] auto inline NativeContextHandle() -> IGraphicsContextCreator::NativeContextHandle
  {
    return m_ContextHandle;
  }

  virtual ~IGraphicsContext() = default;

  [[nodiscard]] virtual auto DrawableSize() -> Size2DI = 0;
  virtual void Resize(const Size2DI &size) = 0;

  virtual void MakeCurrent() = 0;
  virtual void SwapBuffers() = 0;

  virtual void Destroy() = 0;

private:
  IPlatformBackend::NativeWindowHandle m_WindowHandle;
  IGraphicsContextCreator::NativeContextHandle m_ContextHandle;
};

}// namespace JE