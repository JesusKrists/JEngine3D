#include "OpenGLVertexArray.hpp"


#include "JEngine3D/Renderer/IBuffer.hpp"

#include <GL/glew.h>

namespace JE {

static auto ShaderDataTypeToOpenGLBaseType(ShaderDataType type) -> GLenum
{
  switch (type) {
  case ShaderDataType::Float:
  case ShaderDataType::Float2:
  case ShaderDataType::Float3:
  case ShaderDataType::Float4:
  case ShaderDataType::Mat3:
  case ShaderDataType::Mat4:
    return GL_FLOAT;
  case ShaderDataType::Int:
  case ShaderDataType::Int2:
  case ShaderDataType::Int3:
  case ShaderDataType::Int4:
    return GL_INT;
  case ShaderDataType::Bool:
    return GL_BOOL;
  case ShaderDataType::UByte4:
    return GL_UNSIGNED_BYTE;

  case ShaderDataType::None:
    break;
  }

  DEBUGBREAK();
  return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
  ZoneScopedN("OpenGLVertexArray::OpenGLVertexArray");// NOLINT
  glGenVertexArrays(1, &m_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
  ZoneScopedN("OpenGLVertexArray::~OpenGLVertexArray");// NOLINT
  glDeleteVertexArrays(1, &m_RendererID);
}

void OpenGLVertexArray::Bind() const
{
  ZoneScopedN("OpenGLVertexArray::Bind");// NOLINT
  glBindVertexArray(m_RendererID);
}

void OpenGLVertexArray::Unbind() const
{
  ZoneScopedN("OpenGLVertexArray::Unbind");// NOLINT
  glBindVertexArray(0);
}

void OpenGLVertexArray::ConfigureVertexBufferLayout(const BufferLayout &bufferLayout)
{
  ZoneScopedN("OpenGLVertexArray::ConfigureVertexBufferLayout");// NOLINT
  ASSERT(!bufferLayout.Elements().empty(), "Vertex Buffer has no layout!");

  for (const auto &element : bufferLayout) {
    switch (element.Type) {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::UByte4: {
      glEnableVertexAttribArray(m_LocationIndex);
      glVertexAttribPointer(m_LocationIndex,
        static_cast<GLint>(element.ComponentCount()),
        ShaderDataTypeToOpenGLBaseType(element.Type),
        element.Normalize ? GL_TRUE : GL_FALSE,
        static_cast<GLsizei>(bufferLayout.Stride()),
        reinterpret_cast<const void *>(element.Offset));// NOLINT
      m_LocationIndex++;
      break;
    }
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
    case ShaderDataType::Bool: {
      glEnableVertexAttribArray(m_LocationIndex);
      glVertexAttribIPointer(m_LocationIndex,
        static_cast<GLint>(element.ComponentCount()),
        ShaderDataTypeToOpenGLBaseType(element.Type),
        static_cast<GLsizei>(bufferLayout.Stride()),
        reinterpret_cast<const void *>(element.Offset));// NOLINT
      m_LocationIndex++;
      break;
    }
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4: {
      auto count = element.ComponentCount();
      for (size_t i = 0; i < count; i++) {
        glEnableVertexAttribArray(m_LocationIndex);
        glVertexAttribPointer(m_LocationIndex,
          static_cast<GLint>(count),
          ShaderDataTypeToOpenGLBaseType(element.Type),
          element.Normalize ? GL_TRUE : GL_FALSE,
          static_cast<GLsizei>(bufferLayout.Stride()),
          reinterpret_cast<const void *>((element.Offset + sizeof(float) * count * i)));// NOLINT
        // glVertexAttribDivisor(m_LocationIndex, 1); maybe add this later when implementing instanced rendering
        m_LocationIndex++;
      }
      break;
    }
    default:
      DEBUGBREAK();
    }
  }
}

void OpenGLVertexArray::Reset()
{
  ZoneScopedN("OpenGLVertexArray::Reset");// NOLINT
  if (m_RendererID == 0) {
    glGenVertexArrays(1, &m_RendererID);
    ForEach(VertexBuffers(), [&](const IVertexBuffer &buffer) { ConfigureVertexBuffer(buffer); });
    ConfigureIndexBuffer(IndexBuffer());
  }
}

void OpenGLVertexArray::Delete()
{
  ZoneScopedN("OpenGLVertexArray::Delete");// NOLINT
  glDeleteVertexArrays(1, &m_RendererID);
  m_RendererID = 0;
  m_LocationIndex = 0;
}

}// namespace JE