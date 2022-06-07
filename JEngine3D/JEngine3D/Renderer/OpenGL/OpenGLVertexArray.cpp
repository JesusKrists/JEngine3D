#include "OpenGLVertexArray.hpp"

#include "JEngine3D/Core/Base.hpp"
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

OpenGLVertexArray::OpenGLVertexArray() { glGenVertexArrays(1, &m_RendererID); }

OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

void OpenGLVertexArray::Bind() const { glBindVertexArray(m_RendererID); }

void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }

void OpenGLVertexArray::ConfigureVertexBufferLayout(const BufferLayout &bufferLayout)
{
  ASSERT(bufferLayout.Elements().size(), "Vertex Buffer has no layout!");

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
          reinterpret_cast<const void *>((element.Offset + sizeof(float) * count * i)));
        // glVertexAttribDivisor(m_LocationIndex, 1); maybe add this later when implementing instanced rendering
        m_LocationIndex++;
      }
      break;
    }
    default:
      DEBUGBREAK();
    }
  }
}// namespace JE

}// namespace JE