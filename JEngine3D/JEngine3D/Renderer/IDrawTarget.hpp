#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/Types.hpp"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IDrawTarget
{
public:
  virtual ~IDrawTarget() = default;

  virtual void Clear(const Color &clearColor) = 0;

  virtual void DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
    const Vector<uint32_t, MemoryTag::Renderer> &indices) = 0;
};

}// namespace JE