#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Renderer/IBuffer.hpp"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IVertexArray
{
  using VertexBufferContainer = Vector<std::reference_wrapper<const IVertexBuffer>, MemoryTag::Renderer>;

public:
  virtual ~IVertexArray() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void AddVertexBuffer(const IVertexBuffer &vertexBuffer) = 0;
  virtual void SetIndexBuffer(const IIndexBuffer &indexBuffer) = 0;

  [[nodiscard]] virtual auto VertexBuffers() const -> const VertexBufferContainer & = 0;
  [[nodiscard]] virtual auto IndexBuffer() const -> const IIndexBuffer & = 0;
};

}// namespace JE