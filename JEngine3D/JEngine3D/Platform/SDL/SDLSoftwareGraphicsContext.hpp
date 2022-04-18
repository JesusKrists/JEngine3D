#pragma once

#include "JEngine3D/Platform/IGraphicsContext.hpp"
#include "JEngine3D/Core/Types.hpp"// for Size2DI
#include "JEngine3D/Platform/IGraphicsContextCreator.hpp"// for IGraphicsC...
#include "JEngine3D/Platform/IPlatformBackend.hpp"// for IPlatformB...
#include "JEngine3D/Core/Assert.hpp"// for ASSERT_

namespace JE {

class SDLSoftwareGraphicsContext final : public IGraphicsContext
{
public:
  using NativeTextureHandle = void *;

  SDLSoftwareGraphicsContext(IPlatformBackend::NativeWindowHandle windowHandle,
    IGraphicsContextCreator::NativeContextHandle contextHandle);

  [[nodiscard]] auto DrawableSize() -> Size2DI override;
  void Resize(const Size2DI &size) override;

  void MakeCurrent() override;
  void SwapBuffers() override;

  void Destroy() override;

  void Clear();

  [[nodiscard]] inline auto PixelPtr() -> void *
  {
    ASSERT(m_PixelPtr, "Context not set to current");
    return m_PixelPtr;
  }
  [[nodiscard]] inline auto Pitch() const -> int32_t { return m_Pitch; }

  [[nodiscard]] auto RendererSize() -> Size2DI;

private:
  NativeTextureHandle m_Texture;
  void *m_PixelPtr = nullptr;
  int32_t m_Pitch = 0;
};

}// namespace JE