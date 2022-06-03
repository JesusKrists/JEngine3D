#pragma once

#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/MemoryController.hpp"

#include "ISoftwareShader.hpp"
#include "SoftwareVertexArray.hpp"

namespace JE::SoftwareRasterizer {

void Clear(const Color &color, uint32_t *pixelPtr, const Size2DI &bufferSize);

void DrawIndexed(const SoftwareVertexArray &vertexArray,
  size_t indexCount,
  ISoftwareShader &shader,
  uint32_t *pixelPtr,
  const Size2DI &bufferSize);


}// namespace JE::SoftwareRasterizer