#pragma once

#include "JEngine3D/Platform/IGraphicsContext.hpp"
#include "JEngine3D/Core/Types.hpp"// for Size2DI
#include "JEngine3D/Platform/IGraphicsContextCreator.hpp"// for IGraphicsC...
#include "JEngine3D/Platform/IPlatformBackend.hpp"// for IPlatformB...
#include "JEngine3D/Core/Assert.hpp"// for ASSERT_
#include "JEngine3D/Core/MemoryController.hpp"// for MemoryTag

namespace JE {

class SDLSoftwareGraphicsContext final : public IGraphicsContext
{
public:
  using NativeTextureHandle = void *;

  SDLSoftwareGraphicsContext(IPlatformBackend::NativeWindowHandle windowHandle,
    IGraphicsContextCreator::NativeContextHandle contextHandle);

  [[nodiscard]] auto DrawableSize() -> Size2DI override;
  void Resize(const Size2DI &size) override;

  void SwapBuffers() override;

  void Destroy() override;


  [[nodiscard]] inline auto PixelPtr() -> uint32_t *
  {
    ASSERT(m_PixelPtr, "Context not set to current");
    return static_cast<uint32_t *>(m_PixelPtr);
  }

  [[nodiscard]] auto RendererSize() -> Size2DI;

private:
  void MakeContextCurrent() override;

  NativeTextureHandle m_Texture = nullptr;
  void *m_PixelPtr = nullptr;
};

}// namespace JE