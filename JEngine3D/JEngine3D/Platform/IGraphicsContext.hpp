#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Platform/IPlatformBackend.hpp"
#include "JEngine3D/Platform/IGraphicsContextCreator.hpp"
#include "JEngine3D/Core/Types.hpp"

#include "JEngine3D/Renderer/IDrawTarget.hpp"

namespace JE {

class Window;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IGraphicsContext : public IDrawTarget
{
public:
  IGraphicsContext(IPlatformBackend::NativeWindowHandle windowHandle,// NOLINT(bugprone-easily-swappable-parameters)
    IGraphicsContextCreator::NativeContextHandle contextHandle)
    : m_WindowHandle(windowHandle), m_ContextHandle(contextHandle)
  {
    if (s_CurrentContext == nullptr) { s_CurrentContext = this; }
  }

  [[nodiscard]] auto inline NativeWindowHandle() -> IPlatformBackend::NativeWindowHandle { return m_WindowHandle; }
  [[nodiscard]] auto inline NativeContextHandle() -> IGraphicsContextCreator::NativeContextHandle
  {
    return m_ContextHandle;
  }

  static inline auto CurrentContext() -> IGraphicsContext &
  {
    ASSERT(s_CurrentContext != nullptr, "Current Graphics Context cannot be null");
    return *s_CurrentContext;
  }

  void MakeCurrent()
  {
    s_CurrentContext = this;
    MakeContextCurrent();
  }

  // IDrawTarget
  inline void Bind() override
  {
    m_PreviousContext = &CurrentContext();
    MakeCurrent();
  }
  inline void Unbind() override { m_PreviousContext->MakeCurrent(); }

  virtual ~IGraphicsContext() = default;// NOLINT
  [[nodiscard]] virtual auto DrawableSize() -> Size2DI = 0;
  virtual void Resize(const Size2DI &size) = 0;
  virtual void SwapBuffers() = 0;

private:
  virtual void MakeContextCurrent() = 0;

  IPlatformBackend::NativeWindowHandle m_WindowHandle;
  IGraphicsContextCreator::NativeContextHandle m_ContextHandle;
  IGraphicsContext *m_PreviousContext = nullptr;

  static IGraphicsContext *s_CurrentContext;// NOLINT
};

}// namespace JE