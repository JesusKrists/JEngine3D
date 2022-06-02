#pragma once

#include "JEngine3D/Renderer/IRendererAPI.hpp"
#include "JEngine3D/Renderer/IVertexArray.hpp"

namespace JE {

class SDLSoftwareGraphicsContext;
class SoftwareFrameBufferObject;
class SoftwareTexture;
class ISoftwareShader;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class SoftwareRendererAPI final : public IRendererAPI
{
  friend class SDLSoftwareGraphicsContext;
  friend class SoftwareFrameBufferObject;
  friend class SoftwareTexture;
  friend class ISoftwareShader;

public:
  void SetViewport(const RectangleI &viewportRect) override;
  void SetClearColor(const Color &color) override;
  void Clear() override;

  void DrawIndexed(const IVertexArray &vertexArray, uint32_t indexCount = 0) override;

  static auto BoundTexture(uint32_t slot) -> const SoftwareTexture *;

private:
  static void BindGraphicsContext(SDLSoftwareGraphicsContext *context);
  static void BindFrameBuffer(SoftwareFrameBufferObject *frameBuffer);
  static void BindTexture(const SoftwareTexture *texture, uint32_t slot);
  static void BindShader(ISoftwareShader *shader);
};

}// namespace JE