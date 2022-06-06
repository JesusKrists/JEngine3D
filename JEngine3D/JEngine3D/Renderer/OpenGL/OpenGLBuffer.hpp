#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Renderer/IBuffer.hpp"

namespace JE {

class OpenGLVertexBuffer : public IVertexBuffer
{
public:
  explicit OpenGLVertexBuffer(const JE::BufferLayout &layout) : IVertexBuffer(layout) {}

  void Bind() const override {}
  void Unbind() const override {}

  void SetData(const std::span<const uint8_t> &data) override { UNUSED(data); }
};

class OpenGLIndexBuffer : public IIndexBuffer
{
public:
  OpenGLIndexBuffer() = default;
  explicit OpenGLIndexBuffer(const std::span<const uint32_t> &data) { UNUSED(data); }

  void Bind() const override {}
  void Unbind() const override {}

  void SetData(const std::span<const uint32_t> &data) override { UNUSED(data); }

  [[nodiscard]] auto Count() const -> size_t override { return 0; }
};

}// namespace JE