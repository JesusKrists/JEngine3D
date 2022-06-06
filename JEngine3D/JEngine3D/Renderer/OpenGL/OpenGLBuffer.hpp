#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Renderer/IBuffer.hpp"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class OpenGLVertexBuffer final : public IVertexBuffer
{
public:
  explicit OpenGLVertexBuffer(const JE::BufferLayout &layout);
  virtual ~OpenGLVertexBuffer();// NOLINT

  void Bind() const override;
  void Unbind() const override;

  void SetData(const std::span<const uint8_t> &data) override;

private:
  uint32_t m_RendererID = 0;
  size_t m_TotalBufferSize = 0;
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class OpenGLIndexBuffer final : public IIndexBuffer
{
public:
  OpenGLIndexBuffer();
  explicit OpenGLIndexBuffer(const std::span<const uint32_t> &data);
  virtual ~OpenGLIndexBuffer();// NOLINT

  void Bind() const override;
  void Unbind() const override;

  void SetData(const std::span<const uint32_t> &data) override;

  [[nodiscard]] inline auto IndexCount() const -> size_t override { return m_IndexCount; }

private:
  uint32_t m_RendererID = 0;
  size_t m_IndexCount = 0;
  size_t m_TotalBufferSize = 0;
};

}// namespace JE