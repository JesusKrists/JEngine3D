#include "SoftwareTexture.hpp"

#include "SoftwareRendererAPI.hpp"

namespace JE {

void SoftwareTexture::SetData(const std::span<const uint8_t> &data, const Size2DI &textureDimensions)
{
  m_PixelData.clear();
  m_PixelData.insert(std::begin(m_PixelData), std::begin(data), std::end(data));
  m_Size = textureDimensions;
}

void SoftwareTexture::Bind(uint32_t slot) const { SoftwareRendererAPI::BindTexture(this, slot); }

}// namespace JE