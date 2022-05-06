#include "SoftwareRasterizer.hpp"

#include "JEngine3D/Core/Assert.hpp"

namespace JE::SoftwareRasterizer {

void Clear(const Color &color, uint32_t *pixelPtr, const Size2DI bufferSize)
{
  ASSERT(pixelPtr != nullptr, "PixelPtr is null");
  std::fill_n(pixelPtr, bufferSize.Width * bufferSize.Height, color.ToABGR8());
}

}// namespace JE::SoftwareRasterizer