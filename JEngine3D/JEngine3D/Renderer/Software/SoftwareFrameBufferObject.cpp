#include "SoftwareFrameBufferObject.hpp"

#include "JEngine3D/Core/Base.hpp"// for size_t
#include "JEngine3D/Renderer/Software/SoftwareRasterizer.hpp"

namespace JE {

SoftwareFrameBufferObject::SoftwareFrameBufferObject(const Size2DI &size)
  : m_PixelData(static_cast<size_t>(size.Width * size.Height)), m_Size(size)
{}

void SoftwareFrameBufferObject::Clear(const Color &clearColor)
{
  SoftwareRasterizer::Clear(clearColor, m_PixelData.data(), m_Size);
}

void SoftwareFrameBufferObject::DrawVerticesIndexed(const Vector<Vertex, MemoryTag::Renderer> &vertices,
  const Vector<uint32_t, MemoryTag::Renderer> &indices)
{
  SoftwareRasterizer::DrawVerticesIndexed(vertices, indices, m_PixelData.data(), m_Size);
}

void SoftwareFrameBufferObject::Resize(const Size2DI &newSize)
{
  m_PixelData.resize(static_cast<size_t>(newSize.Width * newSize.Height));// NOLINT
  m_Size = newSize;
}

}// namespace JE