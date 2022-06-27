#pragma once

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Renderer/IRendererAPI.hpp>

class TestRendererAPI final : public JE::IRendererAPI
{
public:
  inline void UpdateRendererState() override {}
  inline void SetViewport(const JE::RectangleI &viewport) override { JE::UNUSED(viewport); }
  inline void SetClipRect(const JE::RectangleI &clipRect) override { JE::UNUSED(clipRect); }
  inline void SetClearColor(const JE::Color &color) override { JE::UNUSED(color); }
  inline void Clear() override {}

  inline void BindTexture(uint32_t rendererID, uint8_t slot = 0) override// NOLINT
  {
    JE::UNUSED(rendererID);
    JE::UNUSED(slot);
  }
  inline void DrawIndexed(const JE::IVertexArray &vertexArray, size_t indexCount = 0) override
  {
    JE::UNUSED(vertexArray);
    JE::UNUSED(indexCount);
  }
  inline void DrawIndexed(const JE::IVertexArray &vertexArray, size_t indexCount, size_t indexOffset) override
  {
    JE::UNUSED(vertexArray);
    JE::UNUSED(indexCount);
    JE::UNUSED(indexOffset);
  }
};