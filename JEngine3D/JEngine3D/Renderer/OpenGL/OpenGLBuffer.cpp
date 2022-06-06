#include "OpenGLBuffer.hpp"

#include <GL/glew.h>

namespace JE {


OpenGLVertexBuffer::OpenGLVertexBuffer(const JE::BufferLayout &layout) : IVertexBuffer(layout)
{
  glGenBuffers(1, &m_RendererID);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

void OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::SetData(const std::span<const uint8_t> &data)
{
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  if (m_TotalBufferSize < data.size()) {
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size_bytes()), nullptr, GL_DYNAMIC_DRAW);
    m_TotalBufferSize = data.size();
  }

  glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(data.size_bytes()), data.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


///////////////////////////////////////////////////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer() { glGenBuffers(1, &m_RendererID); }

OpenGLIndexBuffer::OpenGLIndexBuffer(const std::span<const uint32_t> &data)
  : m_IndexCount(data.size()), m_TotalBufferSize(m_IndexCount)
{
  glGenBuffers(1, &m_RendererID);

  // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
  // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size_bytes()), data.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLIndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }

void OpenGLIndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void OpenGLIndexBuffer::SetData(const std::span<const uint32_t> &data)
{
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  if (m_TotalBufferSize < data.size()) {
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size_bytes()), nullptr, GL_DYNAMIC_DRAW);
    m_TotalBufferSize = data.size();
  }

  glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(data.size_bytes()), data.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  m_IndexCount = data.size();
}

}// namespace JE