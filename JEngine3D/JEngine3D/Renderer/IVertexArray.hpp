#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Renderer/IBuffer.hpp"

#include <Tracy.hpp>

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IVertexArray
{
public:
  using VertexBufferContainer = Vector<std::reference_wrapper<const IVertexBuffer>, MemoryTag::Renderer>;

  virtual ~IVertexArray() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void Reset() = 0;
  virtual void Delete() = 0;

  [[nodiscard]] inline auto VertexBuffers() const -> const VertexBufferContainer & { return m_VertexBuffers; }
  inline void AddVertexBuffer(const IVertexBuffer &vertexBuffer)
  {
    ZoneScopedN("IVertexArray::AddVertexBuffer");// NOLINT
    ConfigureVertexBuffer(vertexBuffer);
    m_VertexBuffers.push_back(vertexBuffer);
  }

  [[nodiscard]] inline auto IndexBuffer() const -> const IIndexBuffer &
  {
    ASSERT(m_IndexBuffer != nullptr, "No IndexBuffer set for VAO");
    return *m_IndexBuffer;
  }
  inline void SetIndexBuffer(const IIndexBuffer &indexBuffer)
  {
    ZoneScopedN("IVertexArray::SetIndexBuffer");// NOLINT
    ConfigureIndexBuffer(indexBuffer);
    m_IndexBuffer = &indexBuffer;
  }

protected:
  virtual void ConfigureVertexBufferLayout(const BufferLayout &bufferLayout) = 0;

  inline void ConfigureVertexBuffer(const IVertexBuffer &vertexBuffer)
  {
    Bind();
    vertexBuffer.Bind();
    ConfigureVertexBufferLayout(vertexBuffer.BufferLayout());
    vertexBuffer.Unbind();
    Unbind();
  }

  // cppcheck-suppress functionConst
  inline void ConfigureIndexBuffer(const IIndexBuffer &indexBuffer)// NOLINT
  {
    Bind();
    indexBuffer.Bind();
    Unbind();
  }

private:
  VertexBufferContainer m_VertexBuffers;
  const IIndexBuffer *m_IndexBuffer = nullptr;
};

}// namespace JE