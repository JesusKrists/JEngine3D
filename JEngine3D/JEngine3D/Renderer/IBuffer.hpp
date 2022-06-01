#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Types.hpp"

#include <span>

namespace JE {

enum class ShaderDataType { None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool };

inline auto ShaderDataTypeSize(ShaderDataType type) -> uint32_t
{
  switch (type) {
  case ShaderDataType::Float:
    return 4;
  case ShaderDataType::Float2:
    return 4 * 2;
  case ShaderDataType::Float3:
    return 4 * 3;
  case ShaderDataType::Float4:
    return 4 * 4;
  case ShaderDataType::Mat3:
    return 4 * 3 * 3;
  case ShaderDataType::Mat4:
    return 4 * 4 * 4;
  case ShaderDataType::Int:
    return 4;
  case ShaderDataType::Int2:
    return 4 * 2;
  case ShaderDataType::Int3:
    return 4 * 3;
  case ShaderDataType::Int4:
    return 4 * 4;
  case ShaderDataType::Bool:
    return 1;
  default:
    DEBUGBREAK();
    return 0;
  }
}

struct BufferElement
{
  std::string Name;
  ShaderDataType Type;
  uint32_t Size;
  size_t Offset = 0;
  bool Normalized;

  BufferElement(ShaderDataType type, const std::string_view &name, bool normalized = false)
    : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Normalized(normalized)
  {}

  [[nodiscard]] inline auto GetComponentCount() const -> uint32_t
  {
    switch (Type) {
    case ShaderDataType::Float:
      return 1;
    case ShaderDataType::Float2:
      return 2;
    case ShaderDataType::Float3:
      return 3;
    case ShaderDataType::Float4:
      return 4;
    case ShaderDataType::Mat3:
      return 3;// 3* float3
    case ShaderDataType::Mat4:
      return 4;// 4* float4
    case ShaderDataType::Int:
      return 1;
    case ShaderDataType::Int2:
      return 2;
    case ShaderDataType::Int3:
      return 3;
    case ShaderDataType::Int4:
      return 4;
    case ShaderDataType::Bool:
      return 1;
    default:
      DEBUGBREAK();
      return 0;
    }
  }
};

/*class BufferLayout
{
public:
  BufferLayout() = default;
  BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements) { CalculateOffsetsAndStride(); }

  [[nodiscard]] inline auto GetStride() const -> uint32_t { return m_Stride; }
  [[nodiscard]] auto GetElements() const -> const std::vector<BufferElement> & { return m_Elements; }

  std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
  std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
  std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
  std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
  void CalculateOffsetsAndStride()
  {
    size_t offset = 0;
    m_Stride = 0;
    for (auto &element : m_Elements) {
      element.Offset = offset;
      offset += element.Size;
      m_Stride += element.Size;
    }
  }

private:
  std::vector<BufferElement> m_Elements;
  uint32_t m_Stride = 0;
};*/


// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IVertexBuffer
{
public:
  virtual ~IVertexBuffer() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  virtual void SetData(const std::span<const uint8_t> &data) = 0;

  // TODO(JesusKrists): LAYOUT STUFF
};


}// namespace JE