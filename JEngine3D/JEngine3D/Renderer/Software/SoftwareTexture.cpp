#include "SoftwareTexture.hpp"

#include "SoftwareRendererAPI.hpp"

namespace JE {

void SoftwareTexture::SetData(const uint32_t *data, const Size2DI &size)
{
  m_PixelData.clear();
  m_PixelData.insert(std::begin(m_PixelData), data, &data[size.Width * size.Height]);// NOLINT
  m_Size = size;
}

void SoftwareTexture::Bind(uint32_t slot) const { SoftwareRendererAPI::BindTexture(this, slot); }

}// namespace JE