#pragma once

#include "JEngine3D/Renderer/IRendererAPI.hpp"

namespace JE {

class SDLSoftwareGraphicsContext;
class SoftwareFrameBufferObject;
class SoftwareTexture;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class SoftwareRendererAPI : public IRendererAPI
{
  friend class SDLSoftwareGraphicsContext;
  friend class SoftwareFrameBufferObject;
  friend class SoftwareTexture;

public:
  void SetViewport(const RectangleI &viewportRect) override;
  void SetClearColor(const Color &color) override;
  void Clear() override;

  void DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
    const Vector<uint32_t, MemoryTag::Renderer> &indices) override;

private:
  static void BindGraphicsContext(SDLSoftwareGraphicsContext *context);
  static void BindFrameBuffer(SoftwareFrameBufferObject *frameBuffer);
  static void BindTexture(const SoftwareTexture *texture, uint32_t slot);
};

}// namespace JE