#include "SoftwareRendererAPI.hpp"

#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Platform/SDL/Software/SDLSoftwareGraphicsContext.hpp"
#include "SoftwareFrameBufferObject.hpp"

#include "SoftwareRasterizer.hpp"

namespace JE {

struct SoftwareRendererAPIData
{
  static constexpr auto TEXTURE_SLOT_COUNT = 16;

  Color ClearColor = { 0, 0, 0, 0 };
  SoftwareFrameBufferObject *CurrentFBO = nullptr;
  SDLSoftwareGraphicsContext *CurrentContext = nullptr;
  std::array<const SoftwareTexture *, TEXTURE_SLOT_COUNT> TextureSlots{};
  ISoftwareShader *CurrentShader = nullptr;
};

static SoftwareRendererAPIData Data;// NOLINT

static void StateSanityCheck()
{
  if (Data.CurrentFBO == nullptr) { ASSERT(Data.CurrentContext, "No IDrawTarget bound"); }
}

void SoftwareRendererAPI::SetViewport(const RectangleI &viewportRect) { UNUSED(viewportRect); }

void SoftwareRendererAPI::SetClearColor(const Color &color) { Data.ClearColor = color; }

void SoftwareRendererAPI::Clear()
{
  StateSanityCheck();

  if (Data.CurrentFBO != nullptr) {
    SoftwareRasterizer::Clear(Data.ClearColor, Data.CurrentFBO->PixelPtr(), Data.CurrentFBO->Size());
  } else {
    SoftwareRasterizer::Clear(Data.ClearColor, Data.CurrentContext->PixelPtr(), Data.CurrentContext->DrawableSize());
  }
}

void SoftwareRendererAPI::DrawIndexed(const IVertexArray &vertexArray, uint32_t indexCount)
{
  StateSanityCheck();

  ASSERT(Data.CurrentShader, "No shader bound");
  ASSERT(Data.CurrentVAO, "No VAO bound");

  vertexArray.Bind();
  indexCount = indexCount != 0 ? indexCount : vertexArray.IndexBuffer().Count();

  if (Data.CurrentFBO != nullptr) {
    SoftwareRasterizer::DrawIndexed(
      *Data.CurrentVAO, indexCount, *Data.CurrentShader, Data.CurrentFBO->PixelPtr(), Data.CurrentFBO->Size());
  } else {
    SoftwareRasterizer::DrawIndexed(*Data.CurrentVAO,
      indexCount,
      *Data.CurrentShader,
      Data.CurrentContext->PixelPtr(),
      Data.CurrentContext->DrawableSize());
  }

  vertexArray.Unbind();
}


auto SoftwareRendererAPI::BoundTexture(uint32_t slot) -> const SoftwareTexture *
{
  return Data.TextureSlots[slot];// NOLINT
}

void SoftwareRendererAPI::BindGraphicsContext(SDLSoftwareGraphicsContext *context) { Data.CurrentContext = context; }

void SoftwareRendererAPI::BindFrameBuffer(SoftwareFrameBufferObject *frameBuffer) { Data.CurrentFBO = frameBuffer; }

void SoftwareRendererAPI::BindTexture(const SoftwareTexture *texture, uint32_t slot)
{
  ASSERT(slot < SoftwareRendererAPIData::TEXTURE_SLOT_COUNT, "Texture slot count exceeded");
  Data.TextureSlots[slot] = texture;// NOLINT
}

void SoftwareRendererAPI::BindShader(ISoftwareShader *shader) { Data.CurrentShader = shader; }

}// namespace JE