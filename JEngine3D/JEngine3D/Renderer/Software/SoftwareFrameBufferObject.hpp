#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Renderer/IDrawTarget.hpp"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class SoftwareFrameBufferObject : public IDrawTarget
{
public:
  explicit SoftwareFrameBufferObject(const Size2DI &size = { 0, 0 });

  void Clear(const Color &clearColor) override;

  void DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
    const Vector<uint32_t, MemoryTag::Renderer> &indices) override;
  void DrawVertices(const Vector<Vertex, MemoryTag::Renderer> &vertices) override;

  [[nodiscard]] inline auto PixelPtr() -> uint32_t * { return m_PixelData.data(); };
  [[nodiscard]] inline auto Size() const -> const Size2DI & { return m_Size; };

  void Resize(const Size2DI &newSize);

private:
  Vector<uint32_t, MemoryTag::Renderer> m_PixelData;
  Size2DI m_Size = { 0, 0 };
};

}// namespace JE