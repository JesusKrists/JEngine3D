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

  void MakeCurrent() override;
  void SwapBuffers() override;

  void Destroy() override;


  void Clear(const Color &clearColor) override { UNUSED(clearColor); }

  void DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
    const Vector<uint32_t, MemoryTag::Renderer> &indices) override
  {
    UNUSED(vertices);
    UNUSED(indices);
  }
};

}// namespace JE