#pragma once

#include "JEngine3D/Renderer/ITexture.hpp"

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Renderer/Software/SoftwareRendererAPI.hpp"

namespace JE {

class SoftwareTexture final : public ITexture
{
public:
  inline void SetData(const std::span<const uint8_t> &data, const Size2DI &textureDimensions) override
  {
    m_PixelData.clear();
    m_PixelData.insert(std::begin(m_PixelData), std::begin(data), std::end(data));
    m_Size = textureDimensions;
  }
  [[nodiscard]] inline auto Size() const -> const Size2DI & override { return m_Size; }

  inline void Bind(uint32_t slot) const override { SoftwareRendererAPI::BindTexture(this, slot); }

  [[nodiscard]] inline auto PointerValue() const -> size_t override { return reinterpret_cast<size_t>(this); }// NOLINT


  [[nodiscard]] inline auto PixelPtr() const -> const uint8_t * { return m_PixelData.data(); };

private:
  Vector<uint8_t, MemoryTag::Renderer> m_PixelData;
  Size2DI m_Size = { 0, 0 };
};

}// namespace JE