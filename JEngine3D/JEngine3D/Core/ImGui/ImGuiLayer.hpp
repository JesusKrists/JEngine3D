#pragma once

#include "JEngine3D/Core/ILayer.hpp"
#include "JEngine3D/Renderer/ImGui/ImGuiRenderer.hpp"

#include <filesystem>
#include <imgui.h>

namespace JE {


    class IEvent;


    class ImGuiLayer final : public ILayer
    {
        static constexpr auto IMGUI_INTERNAL_FONT_STRING        = std::string_view{ "ImGui Internal Font" };
        static constexpr auto IMGUI_INTERNAL_FONT_STRING_CUSTOM = std::string_view{ "ImGui Internal Font CUSTOM" };
        struct FontEntry
        {
            explicit FontEntry(const std::string_view& filePath)// NOLINT
                : FilePath(filePath), FontAtlas(CreateScope<ImFontAtlas, MemoryTag::App>())
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

            std::string                            FilePath;
            Scope<ImFontAtlas, MemoryTag::App>     FontAtlas;
            ImFont*                                Font = nullptr;
            Scope<ITexture2D, MemoryTag::Renderer> FontTexture;
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

            std::string                     FilePath;
            float                           PixelSize;
            float                           xAdvance;
            Vector<ImWchar, MemoryTag::App> GlyphRanges;
            bool                            MergeWithPrevious;
        };

        using FontCacheContainer = Vector<Scope<FontEntry, MemoryTag::App>, MemoryTag::App>;
        using MergeFontContainer = Vector<MergeFontEntry, MemoryTag::App>;

    public:
        ImGuiLayer() : ILayer("JEngine ImGuiLayer") {}

        void OnCreate() override;
        void OnDestroy() override;

        void OnUpdate() override;
        void OnImGuiRender() override;

        void OnEvent(JE::IEvent& event) override;

        void                      SetCaptureEvents(bool capture = true);
        [[nodiscard]] inline auto CaptureEvents() const -> bool { return m_CaptureEvents; }


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

        // cppcheck-suppress functionStatic
        void Begin();
        // cppcheck-suppress functionStatic
        void End();

        // cppcheck-suppress functionConst
        [[nodiscard]] inline auto Renderer() -> ImGuiRenderer& { return m_ImGuiRenderer; }

    private:
        void AddMergeFont(ImFontAtlas& fontAtlas);

        ImGuiRenderer      m_ImGuiRenderer;
        FontCacheContainer m_FontCache;
        MergeFontContainer m_MergeFontStack;

        bool m_CaptureEvents = true;
    };

}// namespace JE