#include "OpenGLRendererObjectCreator.hpp"

#include "JEngine3D/Core/MemoryController.hpp"

#include "JEngine3D/Renderer/OpenGL/OpenGLBuffer.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLVertexArray.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLTexture.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLRendererAPI.hpp"

namespace JE {

auto OpenGLRendererObjectCreator::CreateVertexBuffer(const BufferLayout &layout)
  -> Scope<IVertexBuffer, MemoryTag::Renderer>
{
  return CreatePolymorphicScope<OpenGLVertexBuffer, MemoryTag::Renderer, IVertexBuffer>(layout);
}

auto OpenGLRendererObjectCreator::CreateIndexBuffer() -> Scope<IIndexBuffer, MemoryTag::Renderer>
{
  return CreatePolymorphicScope<OpenGLIndexBuffer, MemoryTag::Renderer, IIndexBuffer>();
}

auto OpenGLRendererObjectCreator::CreateIndexBuffer(const std::span<const uint32_t> &indices)
  -> Scope<IIndexBuffer, MemoryTag::Renderer>
{
  return CreatePolymorphicScope<OpenGLIndexBuffer, MemoryTag::Renderer, IIndexBuffer>(indices);
}

auto OpenGLRendererObjectCreator::CreateVertexArray() -> Scope<IVertexArray, MemoryTag::Renderer>
{
  return CreatePolymorphicScope<OpenGLVertexArray, MemoryTag::Renderer, IVertexArray>();
}

auto OpenGLRendererObjectCreator::CreateTexture() -> Scope<ITexture, MemoryTag::Renderer>
{
  return CreatePolymorphicScope<OpenGLTexture, MemoryTag::Renderer, ITexture>();
}

auto OpenGLRendererObjectCreator::CreateAPI() -> Scope<IRendererAPI, MemoryTag::Renderer>
{
  return CreatePolymorphicScope<OpenGLRendererAPI, MemoryTag::Renderer, IRendererAPI>();
}

}// namespace JE