#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Renderer/IBuffer.hpp"

namespace JE {

class SoftwareVertexBuffer final : public IVertexBuffer
{
public:
  explicit SoftwareVertexBuffer(const JE::BufferLayout &layout) : IVertexBuffer(layout) {}

  inline void Bind() const override {}
  inline void Unbind() const override {}

  inline void SetData(const std::span<const uint8_t> &data) override
  {
    m_BufferData.clear();
    m_BufferData.insert(std::begin(m_BufferData), std::begin(data), std::end(data));
  }

  [[nodiscard]] inline auto DataPtr() const -> const uint8_t * { return m_BufferData.data(); }

private:
  Vector<uint8_t, MemoryTag::Renderer> m_BufferData;
};


class SoftwareIndexBuffer final : public IIndexBuffer
{
public:
  SoftwareIndexBuffer() = default;
  explicit SoftwareIndexBuffer(const std::span<const uint32_t> &indices)
    : m_Indices(std::begin(indices), std::end(indices))
  {}

  inline void Bind() const override {}
  inline void Unbind() const override {}

  inline void SetData(const std::span<const uint32_t> &data) override
  {
    m_Indices.clear();
    m_Indices.insert(std::begin(m_Indices), std::begin(data), std::end(data));
  }

  [[nodiscard]] inline auto Count() const -> size_t override { return m_Indices.size(); }


  [[nodiscard]] inline auto DataPtr() const -> const uint8_t *
  {
    return reinterpret_cast<const uint8_t *>(m_Indices.data());// NOLINT
  }

private:
  Vector<uint32_t, MemoryTag::Renderer> m_Indices;
};

}// namespace JE