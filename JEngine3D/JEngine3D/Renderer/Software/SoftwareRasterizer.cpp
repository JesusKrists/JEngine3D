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

/*static void
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
}*/

// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
static auto
  Barycentric(const Position2DI &v0, const Position2DI &v1, const Position2DI &v2, const Position2DI &point)// NOLINT
  -> glm::vec3
{
  glm::vec2 vv0 = { v1.X - v0.X, v1.Y - v0.Y };
  glm::vec2 vv1 = { v2.X - v0.X, v2.Y - v0.Y };
  glm::vec2 vv2 = { point.X - v0.X, point.Y - v0.Y };
  float den = vv0.x * vv1.y - vv1.x * vv0.y;// NOLINT

  float v = (vv2.x * vv1.y - vv1.x * vv2.y) / den;// NOLINT
  float w = (vv0.x * vv2.y - vv2.x * vv0.y) / den;// NOLINT
  float u = 1.0F - v - w;// NOLINT

  return glm::vec3{ u, v, w };
}

static void FillTriangle(const Position2DI &position0,
  const Position2DI &position1,
  const Position2DI &position2,
  ISoftwareShader &shader,
  uint32_t *pixelPtr,
  const Size2DI &bufferSizeMinusOne)
{
  Position2DI boundingBoxStart = { bufferSizeMinusOne.Width, bufferSizeMinusOne.Height };
  Position2DI boundingBoxEnd = { 0, 0 };

  boundingBoxEnd.X = std::min(bufferSizeMinusOne.Width, std::max(boundingBoxEnd.X, position0.X));
  boundingBoxEnd.Y = std::min(bufferSizeMinusOne.Height, std::max(boundingBoxEnd.Y, position0.Y));
  boundingBoxStart.X = std::max(0, std::min(boundingBoxStart.X, position0.X));
  boundingBoxStart.Y = std::max(0, std::min(boundingBoxStart.Y, position0.Y));

  boundingBoxEnd.X = std::min(bufferSizeMinusOne.Width, std::max(boundingBoxEnd.X, position1.X));
  boundingBoxEnd.Y = std::min(bufferSizeMinusOne.Height, std::max(boundingBoxEnd.Y, position1.Y));
  boundingBoxStart.X = std::max(0, std::min(boundingBoxStart.X, position1.X));
  boundingBoxStart.Y = std::max(0, std::min(boundingBoxStart.Y, position1.Y));

  boundingBoxEnd.X = std::min(bufferSizeMinusOne.Width, std::max(boundingBoxEnd.X, position2.X));
  boundingBoxEnd.Y = std::min(bufferSizeMinusOne.Height, std::max(boundingBoxEnd.Y, position2.Y));
  boundingBoxStart.X = std::max(0, std::min(boundingBoxStart.X, position2.X));
  boundingBoxStart.Y = std::max(0, std::min(boundingBoxStart.Y, position2.Y));

  Position2DI point = { -1, -1 };
  for (point.Y = boundingBoxStart.Y; point.Y <= boundingBoxEnd.Y; point.Y++) {// NOLINT
    for (point.X = boundingBoxStart.X; point.X <= boundingBoxEnd.X; point.X++)// NOLINT
    {
      auto bary = Barycentric(position0, position1, position2, point);
      if (bary.x < 0 || bary.y < 0 || bary.z < 0) { continue; }// NOLINT

      shader.FragmentShader(bary, pixelPtr[point.X + point.Y * (bufferSizeMinusOne.Width + 1)]);// NOLINT
    }
  }
}

void Clear(const Color &color, uint32_t *pixelPtr, const Size2DI &bufferSize)
{
  ASSERT(pixelPtr != nullptr, "PixelPtr is null");
  std::fill_n(pixelPtr, bufferSize.Width * bufferSize.Height, color.ToABGR8());
}

static auto IsPointInsideBuffer(const Position2DI &point, const Size2DI &bufferSize) -> bool
{
  return (point.X >= 0 && point.X <= bufferSize.Width) && (point.Y >= 0 && point.Y <= bufferSize.Height);
}

void DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
  const Vector<uint32_t, MemoryTag::Renderer> &indices,
  ISoftwareShader &shader,
  uint32_t *pixelPtr,
  const Size2DI &bufferSize)
{
  ASSERT(pixelPtr != nullptr, "PixelPtr is null");
  ASSERT(indices.size() % 3 == 0, "Missing triangle indices");

  Size2DI bufferSizeMinusOne = { bufferSize.Width - 1, bufferSize.Height - 1 };

  for (size_t i = 0; i < indices.size(); i += 3) {
    bool draw = false;

    const Vertex &vertex0 = vertices[indices[i]];
    const Vertex &vertex1 = vertices[indices[i + 1]];
    const Vertex &vertex2 = vertices[indices[i + 2]];

    auto vertex0Pos = shader.VertexShader(vertex0, 0);
    auto vertex1Pos = shader.VertexShader(vertex1, 1);
    auto vertex2Pos = shader.VertexShader(vertex2, 2);

    Position2DI position0 = {
      static_cast<int32_t>(
        JE::Math::Map(vertex0Pos.x, { -1, 1 }, { 0, static_cast<float>(bufferSizeMinusOne.Width) })),// NOLINT
      static_cast<int32_t>(
        JE::Math::Map(vertex0Pos.y, { -1, 1 }, { static_cast<float>(bufferSizeMinusOne.Height), 0 }))// NOLINT
    };

    if (IsPointInsideBuffer(position0, bufferSizeMinusOne)) { draw = true; }

    Position2DI position1 = {
      static_cast<int32_t>(
        JE::Math::Map(vertex1Pos.x, { -1, 1 }, { 0, static_cast<float>(bufferSizeMinusOne.Width) })),// NOLINT
      static_cast<int32_t>(
        JE::Math::Map(vertex1Pos.y, { -1, 1 }, { static_cast<float>(bufferSizeMinusOne.Height), 0 }))// NOLINT
    };

    if (!draw && IsPointInsideBuffer(position1, bufferSizeMinusOne)) { draw = true; }

    Position2DI position2 = {
      static_cast<int32_t>(
        JE::Math::Map(vertex2Pos.x, { -1, 1 }, { 0, static_cast<float>(bufferSizeMinusOne.Width) })),// NOLINT
      static_cast<int32_t>(
        JE::Math::Map(vertex2Pos.y, { -1, 1 }, { static_cast<float>(bufferSizeMinusOne.Height), 0 }))// NOLINT
    };

    if (!draw && IsPointInsideBuffer(position2, bufferSizeMinusOne)) { draw = true; }

    if (!draw) { continue; }

    FillTriangle(position0, position1, position2, shader, pixelPtr, bufferSizeMinusOne);
  }
}

}// namespace JE::SoftwareRasterizer