#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"

namespace JE {

class OpenGLTexture final : public ITexture
{
public:
  void SetData(const std::span<const uint8_t> &data, const Size2DI &textureDimensions) override
  {
    UNUSED(data);
    m_TextureSize = textureDimensions;
  }
  [[nodiscard]] auto Size() const -> const Size2DI & override { return m_TextureSize; }

  void Bind(uint32_t slot = 0) const override { UNUSED(slot); }

  [[nodiscard]] auto PointerValue() const -> size_t override { return reinterpret_cast<size_t>(this); }// NOLINT

private:
  Size2DI m_TextureSize = { 0, 0 };
};

}// namespace JE