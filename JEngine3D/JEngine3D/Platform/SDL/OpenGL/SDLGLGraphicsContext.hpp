#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Platform/IGraphicsContext.hpp"
#include "JEngine3D/Core/Types.hpp"// for Size2DI
#include "JEngine3D/Core/MemoryController.hpp"// for MemoryTag, Memory...

namespace JE {

class SDLGLGraphicsContext final : public IGraphicsContext
{
  using IGraphicsContext::IGraphicsContext;

public:
  [[nodiscard]] auto DrawableSize() -> Size2DI override;
  inline void Resize(const Size2DI &size) override { JE::UNUSED(size); }

  void SwapBuffers() override;
  void ResetViewport() override;

  void EnableVSync() override;
  void DisableVSync() override;
  auto VSyncEnabled() -> bool override;

private:
  void MakeContextCurrent() override;
};

}// namespace JE