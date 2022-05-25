#include "SoftwareRendererAPI.hpp"

#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Platform/SDL/Software/SDLSoftwareGraphicsContext.hpp"
#include "SoftwareFrameBufferObject.hpp"

#include "SoftwareRasterizer.hpp"

namespace JE {

struct SoftwareRendererAPIData
{
  Color ClearColor = { 0, 0, 0, 0 };
  SoftwareFrameBufferObject *CurrentlyBoundFrameBuffer = nullptr;
  SDLSoftwareGraphicsContext *CurrentlyBoundGraphicsContext = nullptr;
};

static SoftwareRendererAPIData Data;// NOLINT

static void StateSanityCheck()
{
  if (Data.CurrentlyBoundFrameBuffer == nullptr) { ASSERT(Data.CurrentlyBoundGraphicsContext, "No IDrawTarget bound"); }
}

void SoftwareRendererAPI::SetViewport(const RectangleI &viewportRect) { UNUSED(viewportRect); }

void SoftwareRendererAPI::SetClearColor(const Color &color) { Data.ClearColor = color; }

void SoftwareRendererAPI::Clear()
{
  StateSanityCheck();

  if (Data.CurrentlyBoundFrameBuffer != nullptr) {
    SoftwareRasterizer::Clear(
      Data.ClearColor, Data.CurrentlyBoundFrameBuffer->PixelPtr(), Data.CurrentlyBoundFrameBuffer->Size());
  } else {
    SoftwareRasterizer::Clear(Data.ClearColor,
      Data.CurrentlyBoundGraphicsContext->PixelPtr(),
      Data.CurrentlyBoundGraphicsContext->DrawableSize());
  }
}

void SoftwareRendererAPI::DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
  const Vector<uint32_t, MemoryTag::Renderer> &indices)
{
  StateSanityCheck();

  if (Data.CurrentlyBoundFrameBuffer != nullptr) {
    SoftwareRasterizer::DrawVerticesIndexed(
      vertices, indices, Data.CurrentlyBoundFrameBuffer->PixelPtr(), Data.CurrentlyBoundFrameBuffer->Size());
  } else {
    SoftwareRasterizer::DrawVerticesIndexed(vertices,
      indices,
      Data.CurrentlyBoundGraphicsContext->PixelPtr(),
      Data.CurrentlyBoundGraphicsContext->DrawableSize());
  }
}

void SoftwareRendererAPI::BindGraphicsContext(SDLSoftwareGraphicsContext *context)
{
  Data.CurrentlyBoundGraphicsContext = context;
}

void SoftwareRendererAPI::BindFrameBuffer(SoftwareFrameBufferObject *frameBuffer)
{
  Data.CurrentlyBoundFrameBuffer = frameBuffer;
}

}// namespace JE