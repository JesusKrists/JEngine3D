#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Renderer/IVertexArray.hpp"
#include "JEngine3D/Renderer/IRendererObjectCreator.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"
#include "JEngine3D/Renderer/IRendererAPI.hpp"

struct ImDrawData;
struct ImDrawList;
namespace JE {

    // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
    class ImGuiRenderer
    {
    public:
        ImGuiRenderer();
        ~ImGuiRenderer() = default;

        // cppcheck-suppress functionStatic
        void Initialize();
        void RenderDrawData(const ImDrawData& drawData);

    private:
        void SetupRenderState(const ImDrawData& drawData);
        void RenderCommandList(const ImDrawList& drawList, const ImDrawData& drawData);

        Scope<IVertexBuffer, MemoryTag::Renderer> m_VertexBuffer =
        IRendererObjectCreator::Get().CreateVertexBuffer({ { ShaderDataType::Float2, BufferElement::VERTEX_ATTRIBUTE_NAME },
                                                           { ShaderDataType::Float2, BufferElement::UV_ATTRIBUTE_NAME },
                                                           { ShaderDataType::UByte4, BufferElement::COLOR_ATTRIBUTE_NAME, true } });

        Scope<IIndexBuffer, MemoryTag::Renderer> m_IndexBuffer = IRendererObjectCreator::Get().CreateIndexBuffer();
        Scope<IVertexArray, MemoryTag::Renderer> m_VertexArray = IRendererObjectCreator::Get().CreateVertexArray();
        Scope<IShader, MemoryTag::Renderer>      m_Shader;

        RendererState m_ImGuiRendererState = { false, true };
        RendererState m_PreviousRendererState;
    };


}// namespace JE