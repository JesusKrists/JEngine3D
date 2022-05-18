#include "SoftwareRasterizer.hpp"

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/Math.hpp"

#include <cstdlib>// for abs
#include <glm/detail/type_vec3.hpp>// for vec<>::(anonymous)
#include <glm/ext/vector_float3.hpp>// for vec3
#include "JEngine3D/Core/Base.hpp"// for UNUSED, fill_n, size_t

// IWYU pragma: no_include <bits/std_abs.h>

namespace JE::SoftwareRasterizer {

static void
  DrawLine(Position2DI start, Position2DI end, uint32_t abgr8Color, uint32_t *pixelPtr, const Size2DI &bufferSize)
{
  bool steep = false;
  if (std::abs(start.X - end.X) < std::abs(start.Y - end.Y)) {
    std::swap(start.X, start.Y);
    std::swap(end.X, end.Y);
    steep = true;
  }
  if (start.X > end.X) {
    std::swap(start.X, end.X);
    std::swap(start.Y, end.Y);
  }

  int deltaX = end.X - start.X;
  int deltaY = end.Y - start.Y;
  int derror2 = std::abs(deltaY) * 2;
  int error2 = 0;
  int currentY = start.Y;
  for (int currentX = start.X; currentX <= end.X; currentX++) {
    if (steep) {
      pixelPtr[currentY + currentX * bufferSize.Width] = abgr8Color;// NOLINT
    } else {
      pixelPtr[currentX + currentY * bufferSize.Width] = abgr8Color;// NOLINT
    }
    error2 += derror2;
    if (error2 > deltaX) {
      currentY += (end.Y > start.Y ? 1 : -1);
      error2 -= deltaX * 2;
    }
  }
}

void Clear(const Color &color, uint32_t *pixelPtr, const Size2DI &bufferSize)
{
  ASSERT(pixelPtr != nullptr, "PixelPtr is null");
  std::fill_n(pixelPtr, bufferSize.Width * bufferSize.Height, color.ToABGR8());
}


void DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
  const Vector<uint32_t, MemoryTag::Renderer> &indices,
  uint32_t *pixelPtr,
  const Size2DI &bufferSize)
{
  ASSERT(pixelPtr != nullptr, "PixelPtr is null");
  UNUSED(vertices);
  UNUSED(indices);
  UNUSED(pixelPtr);
  UNUSED(bufferSize);
}

void DrawVertices(const Vector<Vertex, MemoryTag::Renderer> &vertices, uint32_t *pixelPtr, const Size2DI &bufferSize)
{
  ASSERT(pixelPtr != nullptr, "PixelPtr is null");

  Size2DI bufferSizeMinusOne = { bufferSize.Width - 1, bufferSize.Height - 1 };

  for (size_t i = 0; i < vertices.size(); i += 3) {
    const Vertex &vertex0 = vertices[i];
    const Vertex &vertex1 = vertices[i + 1];
    const Vertex &vertex2 = vertices[i + 2];

    Position2DI position0 = {
      static_cast<int32_t>(
        JE::Math::Map(vertex0.position.x, { -1, 1 }, { 0, static_cast<float>(bufferSizeMinusOne.Width) })),// NOLINT
      static_cast<int32_t>(
        JE::Math::Map(vertex0.position.y, { -1, 1 }, { static_cast<float>(bufferSizeMinusOne.Height), 0 }))// NOLINT
    };

    Position2DI position1 = {
      static_cast<int32_t>(
        JE::Math::Map(vertex1.position.x, { -1, 1 }, { 0, static_cast<float>(bufferSizeMinusOne.Width) })),// NOLINT
      static_cast<int32_t>(
        JE::Math::Map(vertex1.position.y, { -1, 1 }, { static_cast<float>(bufferSizeMinusOne.Height), 0 }))// NOLINT
    };

    Position2DI position2 = {
      static_cast<int32_t>(
        JE::Math::Map(vertex2.position.x, { -1, 1 }, { 0, static_cast<float>(bufferSizeMinusOne.Width) })),// NOLINT
      static_cast<int32_t>(
        JE::Math::Map(vertex2.position.y, { -1, 1 }, { static_cast<float>(bufferSizeMinusOne.Height), 0 }))// NOLINT
    };

    uint32_t color0 = vertex0.color.ToABGR8();
    uint32_t color1 = vertex1.color.ToABGR8();
    uint32_t color2 = vertex2.color.ToABGR8();

    DrawLine(position0, position1, color0, pixelPtr, bufferSize);
    DrawLine(position1, position2, color1, pixelPtr, bufferSize);
    DrawLine(position2, position0, color2, pixelPtr, bufferSize);
  }
}

}// namespace JE::SoftwareRasterizer