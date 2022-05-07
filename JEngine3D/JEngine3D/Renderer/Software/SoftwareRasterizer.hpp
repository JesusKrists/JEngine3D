#pragma once

#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/MemoryController.hpp"

namespace JE::SoftwareRasterizer {

void Clear(const Color &color, uint32_t *pixelPtr, const Size2DI &bufferSize);

void DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
  const Vector<uint32_t, MemoryTag::Renderer> &indices,
  uint32_t *pixelPtr,
  const Size2DI &bufferSize);

void DrawVertices(const Vector<Vertex, MemoryTag::Renderer> &vertices, uint32_t *pixelPtr, const Size2DI &bufferSize);


}// namespace JE::SoftwareRasterizer