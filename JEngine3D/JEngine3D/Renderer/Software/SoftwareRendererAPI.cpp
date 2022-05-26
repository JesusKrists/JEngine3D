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

void SoftwareRendererAPI::DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
  const Vector<uint32_t, MemoryTag::Renderer> &indices)
{
  StateSanityCheck();

  ASSERT(Data.CurrentShader, "No shader bound");

  if (Data.CurrentFBO != nullptr) {
    SoftwareRasterizer::DrawVerticesIndexed(
      vertices, indices, *Data.CurrentShader, Data.CurrentFBO->PixelPtr(), Data.CurrentFBO->Size());
  } else {
    SoftwareRasterizer::DrawVerticesIndexed(
      vertices, indices, *Data.CurrentShader, Data.CurrentContext->PixelPtr(), Data.CurrentContext->DrawableSize());
  }
}


auto SoftwareRendererAPI::GetTexture(uint32_t slot) -> const SoftwareTexture *
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