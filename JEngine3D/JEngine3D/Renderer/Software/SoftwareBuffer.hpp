#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Renderer/IBuffer.hpp"

namespace JE {

class SoftwareVertexBuffer final : public IVertexBuffer
{
public:
  explicit SoftwareVertexBuffer(const JE::BufferLayout &layout) : IVertexBuffer(layout) {}
  SoftwareVertexBuffer(const JE::BufferLayout &layout, const std::span<const uint8_t> &data)
    : IVertexBuffer(layout), m_BufferData(std::begin(data), std::end(data))
  {}

  void Bind() const override;
  void Unbind() const override;

  inline void SetData(const std::span<const uint8_t> &data) override
  {
    m_BufferData.clear();
    m_BufferData.insert(std::begin(m_BufferData), std::begin(data), std::end(data));
  }

private:
  Vector<uint8_t, MemoryTag::Renderer> m_BufferData;
};


class SoftwareIndexBuffer final : public IIndexBuffer
{
public:
  SoftwareIndexBuffer() = default;
  explicit SoftwareIndexBuffer(const std::span<const size_t> &indices)
    : m_Indices(std::begin(indices), std::end(indices))
  {}

  void Bind() const override;
  void Unbind() const override;

  inline void SetData(const std::span<const size_t> &data) override
  {
    m_Indices.clear();
    m_Indices.insert(std::begin(m_Indices), std::begin(data), std::end(data));
  }

  [[nodiscard]] inline auto Count() const -> size_t override { return m_Indices.size(); }

private:
  Vector<size_t, MemoryTag::Renderer> m_Indices;
};

}// namespace JE