#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Renderer/IDrawTarget.hpp"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class SoftwareFrameBufferObject final : public IDrawTarget
{
public:
  explicit SoftwareFrameBufferObject(const Size2DI &size = { 0, 0 })
    : m_PixelData(static_cast<size_t>(size.Width * size.Height)), m_Size(size)
  {}

  void Bind() override;
  void Unbind() override;

  [[nodiscard]] inline auto PixelPtr() -> uint32_t * { return m_PixelData.data(); };
  [[nodiscard]] inline auto Size() const -> const Size2DI & { return m_Size; };

  void Resize(const Size2DI &newSize);

private:
  Vector<uint32_t, MemoryTag::Renderer> m_PixelData;
  Size2DI m_Size = { 0, 0 };
};

}// namespace JE