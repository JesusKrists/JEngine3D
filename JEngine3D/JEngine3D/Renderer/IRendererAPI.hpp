#pragma once

#include "JEngine3D/Core/MemoryController.hpp"

namespace JE {

struct RectangleI;
struct Color;
struct Vertex;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IRendererAPI
{
public:
  virtual ~IRendererAPI() = default;

  virtual void SetViewport(const RectangleI &viewportRect) = 0;
  virtual void SetClearColor(const Color &color) = 0;
  virtual void Clear() = 0;

  virtual void DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
    const Vector<uint32_t, MemoryTag::Renderer> &indices) = 0;
};

}// namespace JE