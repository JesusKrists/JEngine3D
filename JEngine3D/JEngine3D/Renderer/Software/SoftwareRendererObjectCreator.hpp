#pragma once

#include "JEngine3D/Renderer/IRendererObjectCreator.hpp"

#include "SoftwareRendererAPI.hpp"
#include "SoftwareBuffer.hpp"
#include "SoftwareVertexArray.hpp"
#include "SoftwareTexture.hpp"

namespace JE {

class SoftwareRendererObjectCreator final : public IRendererObjectCreator
{
public:
  [[nodiscard]] inline auto CreateVertexBuffer(const BufferLayout &layout)
    -> Scope<IVertexBuffer, MemoryTag::Renderer> override
  {
    return CreatePolymorphicScope<SoftwareVertexBuffer, MemoryTag::Renderer, IVertexBuffer>(layout);
  }
  [[nodiscard]] inline auto CreateIndexBuffer() -> Scope<IIndexBuffer, MemoryTag::Renderer> override
  {
    return CreatePolymorphicScope<SoftwareIndexBuffer, MemoryTag::Renderer, IIndexBuffer>();
  }
  [[nodiscard]] inline auto CreateIndexBuffer(const std::span<const uint32_t> &indices)
    -> Scope<IIndexBuffer, MemoryTag::Renderer> override
  {
    return CreatePolymorphicScope<SoftwareIndexBuffer, MemoryTag::Renderer, IIndexBuffer>(indices);
  }
  [[nodiscard]] inline auto CreateVertexArray() -> Scope<IVertexArray, MemoryTag::Renderer> override
  {

    return CreatePolymorphicScope<SoftwareVertexArray, MemoryTag::Renderer, IVertexArray>();
  }

  [[nodiscard]] inline auto CreateTexture() -> Scope<ITexture, MemoryTag::Renderer> override
  {
    return CreatePolymorphicScope<SoftwareTexture, MemoryTag::Renderer, ITexture>();
  }

private:
  [[nodiscard]] inline auto CreateAPI() -> Scope<IRendererAPI, MemoryTag::Renderer> override
  {
    return CreatePolymorphicScope<SoftwareRendererAPI, MemoryTag::Renderer, IRendererAPI>();
  }
};

}// namespace JE