#pragma once

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/MemoryController.hpp"

#include <span>

namespace JE {

enum class ShaderDataType { None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, UByte4, Bool };

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
  case ShaderDataType::UByte4:
    return 4;
  case ShaderDataType::Bool:
    return 1;
  default:
    DEBUGBREAK();
    return 0;
  }
}

struct BufferElement
{
  static constexpr auto VERTEX_ATTRIBUTE_NAME = std::string_view{ "a_Position" };
  static constexpr auto UV_ATTRIBUTE_NAME = std::string_view{ "a_UV" };
  static constexpr auto COLOR_ATTRIBUTE_NAME = std::string_view{ "a_Color" };

  std::string Name;
  ShaderDataType Type;
  uint32_t Size;
  uint32_t Offset = 0;
  bool Normalize;

  BufferElement(ShaderDataType type, const std::string_view &name, bool normalize = false)
    : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Normalize(normalize)
  {}

  [[nodiscard]] inline auto ComponentCount() const -> size_t
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
    case ShaderDataType::Int4:// NOLINT
      return 4;
    case ShaderDataType::UByte4:
      return 4;
    case ShaderDataType::Bool:
      return 1;
    default:
      DEBUGBREAK();
      return 0;
    }
  }
};

class BufferLayout
{
  using ElementContainer = Vector<BufferElement, MemoryTag::Renderer>;

public:
  // cppcheck-suppress noExplicitConstructor
  BufferLayout(const std::initializer_list<BufferElement> &elements) : m_Elements(elements)
  {
    CalculateOffsetsAndStride();
  }

  [[nodiscard]] inline auto Stride() const -> uint32_t { return m_Stride; }
  [[nodiscard]] inline auto Elements() const -> const ElementContainer & { return m_Elements; }

  [[nodiscard]] inline auto begin() -> ElementContainer::iterator { return std::begin(m_Elements); }
  [[nodiscard]] inline auto end() -> ElementContainer::iterator { return std::end(m_Elements); }
  [[nodiscard]] inline auto begin() const -> ElementContainer::const_iterator { return std::begin(m_Elements); }
  [[nodiscard]] inline auto end() const -> ElementContainer::const_iterator { return std::end(m_Elements); }

private:
  void CalculateOffsetsAndStride()
  {
    uint32_t offset = 0;
    m_Stride = 0;
    for (auto &element : m_Elements) {
      element.Offset = offset;
      offset += element.Size;
    }
    m_Stride = offset;
  }

  ElementContainer m_Elements;
  uint32_t m_Stride = 0;
};


// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IVertexBuffer
{
public:
  virtual ~IVertexBuffer() = default;

  explicit IVertexBuffer(const JE::BufferLayout &layout) : m_BufferLayout(layout) {}// NOLINT

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetData(const std::span<const std::byte> &data) = 0;

  [[nodiscard]] inline auto BufferLayout() const -> const JE::BufferLayout & { return m_BufferLayout; }

private:
  JE::BufferLayout m_BufferLayout;
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IIndexBuffer
{
public:
  virtual ~IIndexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetData(const std::span<const uint32_t> &data) = 0;

  [[nodiscard]] virtual auto IndexCount() const -> size_t = 0;
};


}// namespace JE