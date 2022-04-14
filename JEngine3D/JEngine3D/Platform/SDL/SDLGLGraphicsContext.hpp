#pragma once

#include "JEngine3D/Platform/IGraphicsContext.hpp"
#include "JEngine3D/Core/Types.hpp"// for Size2DI

namespace JE {

class SDLGLGraphicsContext : public IGraphicsContext
{
  using IGraphicsContext::IGraphicsContext;

public:
  [[nodiscard]] auto DrawableSize() -> Size2DI override;

  void MakeCurrent() override;
  void SwapBuffers() override;

  void Destroy() override;
};

}// namespace JE