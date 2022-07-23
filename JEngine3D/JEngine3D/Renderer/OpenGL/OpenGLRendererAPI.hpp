#pragma once

#include "JEngine3D/Renderer/IRendererAPI.hpp"

namespace JE {

    class OpenGLRendererAPI final : public IRendererAPI
    {
    public:
        OpenGLRendererAPI();

        void UpdateRendererState() override;
        void SetViewport(const RectangleI& viewport) override;
        void SetClipRect(const RectangleI& clipRect) override;
        void SetClearColor(const Color& color) override;
        void Clear() override;

        void BindTexture(uint32_t rendererID, uint8_t slot = 0) override;
        void DrawIndexed(const IVertexArray& vertexArray, size_t indexCount = 0) override;
        void DrawIndexed(const IVertexArray& vertexArray, size_t indexCount, size_t indexOffset) override;
    };

}// namespace JE