#pragma once

#include "JEngine3D/Renderer/IRendererObjectCreator.hpp"

namespace JE {

class OpenGLRendererObjectCreator : public IRendererObjectCreator
{
public:
  [[nodiscard]] auto CreateVertexBuffer(const BufferLayout &layout)
    -> Scope<IVertexBuffer, MemoryTag::Renderer> override;
  [[nodiscard]] auto CreateIndexBuffer() -> Scope<IIndexBuffer, MemoryTag::Renderer> override;
  [[nodiscard]] auto CreateIndexBuffer(const std::span<const uint32_t> &indices)
    -> Scope<IIndexBuffer, MemoryTag::Renderer> override;
  [[nodiscard]] auto CreateVertexArray() -> Scope<IVertexArray, MemoryTag::Renderer> override;

  [[nodiscard]] auto CreateTexture() -> Scope<ITexture, MemoryTag::Renderer> override;

private:
  [[nodiscard]] auto CreateAPI() -> Scope<IRendererAPI, MemoryTag::Renderer> override;
};

}// namespace JE