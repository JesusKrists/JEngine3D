#pragma once

#include "JEngine3D/Core/Types.hpp"

namespace JE::SoftwareRasterizer {

void Clear(const Color &color, uint32_t *pixelPtr, const Size2DI bufferSize);


}// namespace JE::SoftwareRasterizer