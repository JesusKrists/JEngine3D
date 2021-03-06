#pragma once

#include "JEngine3D/Renderer/IRendererAPI.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"
#include "JEngine3D/Renderer/IShader.hpp"
#include "JEngine3D/Renderer/IBuffer.hpp"
#include "JEngine3D/Renderer/IVertexArray.hpp"
#include "JEngine3D/Renderer/IRendererObjectCreator.hpp"

namespace JE {

    class IDrawTarget;

    class Renderer2D
    {
    public:
        static constexpr size_t MAX_TRIANGLES_PER_BATCH = 50000;
        static constexpr size_t MAX_TRIANGLE_INDICES    = MAX_TRIANGLES_PER_BATCH * 3;
        static constexpr size_t MAX_TEXTURE_SLOTS       = 16;

        static auto Create() -> Scope<Renderer2D, MemoryTag::App> { return CreateScope<Renderer2D, MemoryTag::App>(); }

        Renderer2D();

        void NewFrame();

        void BeginBatch(IDrawTarget* target = nullptr);
        void EndBatch();

        void DrawTriangle(const Vertex& vertex0, const Vertex& vertex1, const Vertex& vertex2);
        void DrawTriangle(Vertex& vertex0, Vertex& vertex1, Vertex& vertex2, const ITexture2D& texture);
        void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Color& color);
        void DrawQuad(const glm::vec3&  position,
                      const glm::vec2&  size,
                      const ITexture2D& texture,
                      const Color&      tintColor = Color{ 1.0F, 1.0F, 1.0F, 1.0F });

        void DrawQuad(const glm::mat4& transform, const Color& color);
        void DrawQuad(const glm::mat4& transform, const ITexture2D& texture, const Color& tintColor = Color{ 1.0F, 1.0F, 1.0F, 1.0F });

        [[nodiscard]] inline auto TrianglesPerBatch() const -> size_t { return Data.TrianglesPerBatch; }
        inline void               SetTrianglesPerBatch(size_t numTriangles)
        {
            ASSERT(numTriangles != 0, "Cannot have 0 triangles in a batch");
            Data.TrianglesPerBatch = std::min(numTriangles, MAX_TRIANGLES_PER_BATCH);
        }

        [[nodiscard]] inline auto FrameTriangleVertexCount() const -> size_t { return Data.Stats.FrameTriangleVertexCount; }
        [[nodiscard]] inline auto FrameTriangleIndexCount() const -> size_t { return Data.Stats.FrameTriangleIndexCount; }
        [[nodiscard]] inline auto FrameTriangleCount() const -> size_t { return Data.Stats.FrameTriangleIndexCount / 3; }
        [[nodiscard]] inline auto FrameDrawCalls() const -> size_t { return Data.Stats.FrameDrawCalls; }

    private:
        [[nodiscard]] inline auto TriangleCount() const -> size_t { return Data.TriangleIndices.size() / 3; }

        void InitializeBatch(IDrawTarget* target);
        void Flush();
        void NextBatch();
        auto PushTexture(const ITexture2D& texture) -> int8_t;

        struct Renderer2DData
        {
            struct Stats
            {
                size_t FrameTriangleVertexCount = 0;
                size_t FrameTriangleIndexCount  = 0;
                size_t FrameDrawCalls           = 0;
            } Stats;

            size_t TrianglesPerBatch = MAX_TRIANGLES_PER_BATCH;

            bool         BatchBegun = false;
            IDrawTarget* Target     = nullptr;

            Vector<Vertex, MemoryTag::Renderer>              TriangleVertices;
            Vector<uint32_t, MemoryTag::Renderer>            TriangleIndices;
            std::array<const ITexture2D*, MAX_TEXTURE_SLOTS> TextureSlots{};
            int8_t                                           TextureSlotIndex = -1;

            Scope<IVertexBuffer, MemoryTag::Renderer> VertexBuffer =
            IRendererObjectCreator::Get().CreateVertexBuffer({ { ShaderDataType::Float3, BufferElement::VERTEX_ATTRIBUTE_NAME },
                                                               { ShaderDataType::Float4, BufferElement::COLOR_ATTRIBUTE_NAME },
                                                               { ShaderDataType::Float2, BufferElement::UV_ATTRIBUTE_NAME },
                                                               { ShaderDataType::Int, "a_TexIndex" } });
            Scope<IIndexBuffer, MemoryTag::Renderer> IndexBuffer = IRendererObjectCreator::Get().CreateIndexBuffer();
            Scope<IVertexArray, MemoryTag::Renderer> VertexArray = IRendererObjectCreator::Get().CreateVertexArray();
            Scope<IShader, MemoryTag::Renderer>      Shader;
        };// namespace JE

        Renderer2DData Data;
    };

}// namespace JE