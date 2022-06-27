#include "OpenGLBuffer.hpp"

#include <GL/glew.h>

#include <Tracy.hpp>

namespace JE {


OpenGLVertexBuffer::OpenGLVertexBuffer(const JE::BufferLayout &layout) : IVertexBuffer(layout)
{
  ZoneScopedN("OpenGLVertexBuffer::OpenGLVertexBuffer");// NOLINT
  glGenBuffers(1, &m_RendererID);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
  ZoneScopedN("OpenGLVertexBuffer::~OpenGLVertexBuffer");// NOLINT
  glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind() const
{
  ZoneScopedN("OpenGLVertexBuffer::Bind");// NOLINT
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const
{
  ZoneScopedN("OpenGLVertexBuffer::Unbind");// NOLINT
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const std::span<const std::byte> &data)
{
  ZoneScopedN("OpenGLVertexBuffer::SetData");// NOLINT

  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  if (m_TotalBufferSize < data.size()) {
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size_bytes()), nullptr, GL_DYNAMIC_DRAW);
    m_TotalBufferSize = data.size();
  }

  glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(data.size_bytes()), data.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


///////////////////////////////////////////////////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer()
{
  ZoneScopedN("OpenGLIndexBuffer::OpenGLIndexBuffer");// NOLINT
  glGenBuffers(1, &m_RendererID);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(const std::span<const uint32_t> &data)
  : IIndexBuffer(data.size()), m_TotalBufferSize(IndexCount())
{
  ZoneScopedN("OpenGLIndexBuffer::OpenGLIndexBuffer(std::span data)");// NOLINT
  glGenBuffers(1, &m_RendererID);

  // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
  // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size_bytes()), data.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
  ZoneScopedN("OpenGLIndexBuffer::~OpenGLIndexBuffer");// NOLINT
  glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind() const
{
  ZoneScopedN("OpenGLIndexBuffer::Bind");// NOLINT
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const
{
  ZoneScopedN("OpenGLIndexBuffer::Unbind");// NOLINT
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLIndexBuffer::UploadData(const std::span<const uint32_t> &data)
{
  ZoneScopedN("OpenGLIndexBuffer::UploadData");// NOLINT
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  if (m_TotalBufferSize < data.size()) {
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size_bytes()), nullptr, GL_DYNAMIC_DRAW);
    m_TotalBufferSize = data.size();
  }

  glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(data.size_bytes()), data.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}// namespace JE