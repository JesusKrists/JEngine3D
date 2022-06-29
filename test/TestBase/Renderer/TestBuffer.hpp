#pragma once

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Renderer/IBuffer.hpp>

class TestVertexBuffer final : public JE::IVertexBuffer
{
public:
  using JE::IVertexBuffer::IVertexBuffer;

  inline void Bind() const override {}
  inline void Unbind() const override {}

  inline void SetData(const std::span<const std::byte> &data) override { JE::UNUSED(data); }
};

class TestIndexBuffer final : public JE::IIndexBuffer
{
public:
  TestIndexBuffer() = default;
  explicit TestIndexBuffer(const std::span<const uint32_t> &data) : JE::IIndexBuffer(data.size()) {}

  inline void Bind() const override {}
  inline void Unbind() const override {}

private:
  inline void UploadData(const std::span<const uint32_t> &data) override { JE::UNUSED(data); }
};