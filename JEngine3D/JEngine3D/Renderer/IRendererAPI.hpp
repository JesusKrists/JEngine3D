#pragma once

#include "JEngine3D/Core/MemoryController.hpp"

namespace JE {

struct RectangleI;
struct Color;
struct Vertex;
class IVertexArray;

struct RendererState
{
  bool DepthTest = true;
  bool ClipTest = false;
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IRendererAPI
{
public:
  virtual ~IRendererAPI() = default;

  inline void SetRendererState(const JE::RendererState &state)
  {
    m_State = state;
    UpdateRendererState();
  };
  [[nodiscard]] inline auto RendererState() const -> const JE::RendererState & { return m_State; };

  virtual void UpdateRendererState() = 0;
  virtual void SetViewport(const RectangleI &viewport) = 0;
  virtual void SetClipRect(const RectangleI &clipRect) = 0;
  virtual void SetClearColor(const Color &color) = 0;
  virtual void Clear() = 0;

  virtual void BindTexture(uint32_t rendererID, uint8_t slot = 0) = 0;
  virtual void DrawIndexed(const IVertexArray &vertexArray, size_t indexCount = 0) = 0;
  virtual void DrawIndexed(const IVertexArray &vertexArray, size_t indexCount, size_t indexByteOffset) = 0;

private:
  JE::RendererState m_State;
};

}// namespace JE