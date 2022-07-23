#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Renderer/IVertexArray.hpp"
#include "JEngine3D/Renderer/IRendererObjectCreator.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"
#include "JEngine3D/Renderer/IRendererAPI.hpp"

#include <filesystem>

#include <imgui.h>

namespace JE {

    struct FontEntry
    {
        explicit FontEntry(const std::string_view& filePath)// NOLINT
            : FilePath(filePath), FontAtlas(CreateScope<ImFontAtlas, MemoryTag::Renderer>())
        {}

        void BuildFontAtlas()
        {
            uint8_t* textureData = nullptr;
            int      fontWidth   = 0;
            int      fontHeight  = 0;
            FontAtlas->GetTexDataAsRGBA32(&textureData, &fontWidth, &fontHeight);

            FontTexture = IRendererObjectCreator::Get().CreateTexture(
            FilePath,
            { reinterpret_cast<const std::byte*>(textureData), static_cast<size_t>(fontWidth * fontHeight * 4) },// NOLINT
            { fontWidth, fontHeight },
            TextureFormat::RGBA8);
            FontAtlas->TexID = FontTexture->RendererID();
        }

        std::string                             FilePath;
        Scope<ImFontAtlas, MemoryTag::Renderer> FontAtlas;
        ImFont*                                 Font = nullptr;
        Scope<ITexture2D, MemoryTag::Renderer>  FontTexture;
    };

    struct MergeFontEntry
    {
        MergeFontEntry(const std::string_view&         filePath,
                       float                           pixelSize,// NOLINT
                       float                           xadvance,
                       const std::span<const ImWchar>& glyphRanges,
                       bool                            mergeWithPrevious)
            : FilePath(filePath), PixelSize(pixelSize), xAdvance(xadvance), MergeWithPrevious(mergeWithPrevious)
        {
            GlyphRanges.assign(std::begin(glyphRanges), std::end(glyphRanges));
            GlyphRanges.push_back(0);
        }

        std::string                          FilePath;
        float                                PixelSize;
        float                                xAdvance;
        Vector<ImWchar, MemoryTag::Renderer> GlyphRanges;
        bool                                 MergeWithPrevious;
    };


    // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
    class ImGuiRenderer
    {
        using FontCacheContainer = Vector<Scope<FontEntry, MemoryTag::Renderer>, MemoryTag::Renderer>;
        using MergeFontContainer = Vector<MergeFontEntry, MemoryTag::Renderer>;

        static constexpr auto IMGUI_INTERNAL_FONT_STRING        = std::string_view{ "ImGui Internal Font" };
        static constexpr auto IMGUI_INTERNAL_FONT_STRING_CUSTOM = std::string_view{ "ImGui Internal Font CUSTOM" };

    public:
        ImGuiRenderer();
        ~ImGuiRenderer() = default;

        // cppcheck-suppress functionStatic
        void Initialize();

        auto AddFont(const std::filesystem::path& fontFilePath,
                     float                        pixelSize,
                     const ImFontConfig*          fontCfg     = nullptr,
                     const ImWchar*               glyphRanges = nullptr) -> ImFont*;
        auto AddImGuiInternalFont(const ImFontConfig* fontCfg = nullptr) -> ImFont*;
        void PushMergeFont(const std::filesystem::path&    fontFilePath,
                           float                           pixelSize,
                           float                           xAdvance,
                           const std::span<const ImWchar>& glyphRanges,
                           bool                            mergeWithPrevious = false);
        void PopMergeFont();


        void RenderDrawData(const ImDrawData& drawData);

    private:
        void AddMergeFont(ImFontAtlas& fontAtlas);

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

        FontCacheContainer m_FontCache;
        MergeFontContainer m_MergeFontStack;
    };


}// namespace JE