#pragma once

#include "JEngine3D/Renderer/IRendererAPI.hpp"

namespace JE {

class OpenGLRendererAPI final : public IRendererAPI
{
public:
  OpenGLRendererAPI();

  void SetViewport(const RectangleI &viewport) override;
  void SetClearColor(const Color &color) override;
  void Clear() override;

  void DrawIndexed(const IVertexArray &vertexArray, size_t indexCount = 0) override;
};

}// namespace JE