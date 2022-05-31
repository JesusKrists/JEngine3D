#pragma once

#include "JEngine3D/Renderer/ITexture.hpp"

#include "JEngine3D/Core/MemoryController.hpp"

namespace JE {

class SoftwareTexture : public ITexture
{
public:
  void SetData(const std::span<const uint8_t> &data, const Size2DI &textureDimensions) override;
  [[nodiscard]] inline auto Size() const -> const Size2DI & override { return m_Size; }

  void Bind(uint32_t slot = 0) const override;

  [[nodiscard]] auto RendererID() const -> size_t override { return reinterpret_cast<size_t>(this); }// NOLINT


  [[nodiscard]] inline auto PixelPtr() const -> const uint8_t * { return m_PixelData.data(); };

private:
  Vector<uint8_t, MemoryTag::Renderer> m_PixelData;
  Size2DI m_Size = { 0, 0 };
};

}// namespace JE