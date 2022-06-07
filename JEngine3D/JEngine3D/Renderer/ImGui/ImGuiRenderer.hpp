#pragma once

#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Renderer/IVertexArray.hpp"
#include "JEngine3D/Renderer/IRendererObjectCreator.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"

struct ImDrawData;
struct ImDrawList;
struct ImVec2;

namespace JE {


// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class ImGuiRenderer
{
public:
  ImGuiRenderer();
  ~ImGuiRenderer() = default;

  void Initialize();
  void RenderDrawData(ImDrawData *drawData);

private:
  // cppcheck-suppress functionStatic
  void SetupRenderState(const RectangleI &viewport);
  // cppcheck-suppress functionStatic
  void RenderCommandList(const ImDrawList *drawList, const RectangleI &viewport, const ImVec2 &clipScale);

  Scope<IVertexBuffer, MemoryTag::Renderer> m_VertexBuffer =
    IRendererObjectCreator::Get().CreateVertexBuffer({ { ShaderDataType::Float2, BufferElement::VERTEX_ATTRIBUTE_NAME },
      { ShaderDataType::Float2, BufferElement::UV_ATTRIBUTE_NAME },
      { ShaderDataType::UByte4, BufferElement::COLOR_ATTRIBUTE_NAME, true } });

  Scope<IIndexBuffer, MemoryTag::Renderer> m_IndexBuffer = IRendererObjectCreator::Get().CreateIndexBuffer();
  Scope<IVertexArray, MemoryTag::Renderer> m_VertexArray = IRendererObjectCreator::Get().CreateVertexArray();
  Scope<ITexture2D, MemoryTag::Renderer> m_FontTexture;
  Scope<IShader, MemoryTag::Renderer> m_Shader;
};


}// namespace JE