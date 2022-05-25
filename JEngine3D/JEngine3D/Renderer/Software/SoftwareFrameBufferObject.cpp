#include "SoftwareFrameBufferObject.hpp"

#include "JEngine3D/Core/Base.hpp"// for size_t
#include "JEngine3D/Renderer/Software/SoftwareRendererAPI.hpp"

namespace JE {

void SoftwareFrameBufferObject::Bind() { SoftwareRendererAPI::BindFrameBuffer(this); }

void SoftwareFrameBufferObject::UnBind() { SoftwareRendererAPI::BindFrameBuffer(nullptr); }

void SoftwareFrameBufferObject::Resize(const Size2DI &newSize)
{
  m_PixelData.resize(static_cast<size_t>(newSize.Width * newSize.Height));// NOLINT
  m_Size = newSize;
}

}// namespace JE