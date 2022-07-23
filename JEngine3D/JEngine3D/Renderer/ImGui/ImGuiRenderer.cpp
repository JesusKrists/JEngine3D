#include "ImGuiRenderer.hpp"

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <imgui.h>

#include <Tracy.hpp>

namespace JE {

    static constexpr std::string_view VERTEX_SHADER_SOURCE = "#version 330 core\n"
                                                             "layout (location = 0) in vec2 a_Position;\n"
                                                             "layout (location = 1) in vec2 a_UV;\n"
                                                             "layout (location = 2) in vec4 a_Color;\n"
                                                             "out vec4 Frag_Color;\n"
                                                             "out vec2 Frag_UV;\n"
                                                             "uniform mat4 u_OrthoProjection;\n"
                                                             "void main()\n"
                                                             "{\n"
                                                             "    Frag_Color = a_Color;\n"
                                                             "    Frag_UV = a_UV;\n"
                                                             "    gl_Position = u_OrthoProjection * vec4(a_Position.xy, 0, 1);\n"
                                                             "}\n";

    static constexpr std::string_view FRAGMENT_SHADER_SOURCE = "#version 330 core\n"
                                                               "layout (location = 0) out vec4 Out_Color;\n"
                                                               "in vec4 Frag_Color;\n"
                                                               "in vec2 Frag_UV;\n"
                                                               "uniform sampler2D u_Texture;\n"
                                                               "void main()\n"
                                                               "{\n"
                                                               "    Out_Color = Frag_Color * texture(u_Texture, Frag_UV);\n"
                                                               "}\n";

    ImGuiRenderer::ImGuiRenderer()
    {
        ZoneScopedN("ImGuiRenderer::ImGuiRenderer");// NOLINT
        m_VertexArray->AddVertexBuffer(*m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(*m_IndexBuffer);

        m_Shader = IRendererObjectCreator::Get().CreateShader("JEngine3D ImGui Shader", VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
    }

    void ImGuiRenderer::Initialize()// NOLINT(readability-convert-member-functions-to-static)
    {
        ZoneScopedN("ImGuiRenderer::Initialize");// NOLINT
        ImGui::GetIO().Fonts->Build();
    }

    auto ImGuiRenderer::AddFont(const std::filesystem::path& fontFilePath,
                                float                        pixelSize,
                                const ImFontConfig*          fontCfg,
                                const ImWchar*               glyphRanges) -> ImFont*
    {
        auto fontIt = FindIf(m_FontCache, [&](const Scope<FontEntry, MemoryTag::Renderer>& entry) {
            return entry->FilePath == fontFilePath.generic_string() && entry->Font->FontSize == pixelSize;
        });
        if (fontIt == std::end(m_FontCache)) {
            m_FontCache.push_back(CreateScope<FontEntry, MemoryTag::Renderer>(fontFilePath.generic_string()));
            auto& entry = m_FontCache.back();

            entry->Font = entry->FontAtlas->AddFontFromFileTTF(entry->FilePath.c_str(), pixelSize, fontCfg, glyphRanges);
            AddMergeFont(*entry->FontAtlas);

            entry->BuildFontAtlas();

            return entry->Font;
        }

        return (*fontIt)->Font;
    }

    auto ImGuiRenderer::AddImGuiInternalFont(const ImFontConfig* fontCfg) -> ImFont*
    {
        auto fontIt = FindIf(m_FontCache, [&](const Scope<FontEntry, MemoryTag::Renderer>& entry) {
            if (fontCfg != nullptr) {
                return entry->FilePath == IMGUI_INTERNAL_FONT_STRING_CUSTOM && entry->Font->FontSize == fontCfg->SizePixels;
            }
            return entry->FilePath == IMGUI_INTERNAL_FONT_STRING;
        });

        if (fontIt == std::end(m_FontCache)) {
            m_FontCache.push_back(CreateScope<FontEntry, MemoryTag::Renderer>(fontCfg == nullptr ? IMGUI_INTERNAL_FONT_STRING
                                                                                                 : IMGUI_INTERNAL_FONT_STRING_CUSTOM));
            auto& entry = m_FontCache.back();

            entry->Font = entry->FontAtlas->AddFontDefault(fontCfg);
            AddMergeFont(*entry->FontAtlas);

            entry->BuildFontAtlas();

            return entry->Font;
        }

        return (*fontIt)->Font;
    }

    void ImGuiRenderer::PushMergeFont(const std::filesystem::path&    fontFilePath,
                                      float                           pixelSize,
                                      float                           xAdvance,
                                      const std::span<const ImWchar>& glyphRanges,
                                      bool                            mergeWithPrevious)
    {
        auto fontIt = FindIf(m_MergeFontStack, [&](const MergeFontEntry& entry) {
            return entry.FilePath == fontFilePath.generic_string() && entry.PixelSize == pixelSize && entry.xAdvance == xAdvance
                   && entry.MergeWithPrevious == mergeWithPrevious;
        });
        if (fontIt == std::end(m_MergeFontStack)) {
            m_MergeFontStack.emplace_back(fontFilePath.generic_string(), pixelSize, xAdvance, glyphRanges, mergeWithPrevious);
        } else {
            Logger::CoreLogger().error("PushMergeFont: Font already in merge stack");
        }
    }

    void ImGuiRenderer::PopMergeFont()
    {
        if (!m_MergeFontStack.empty()) { m_MergeFontStack.erase(--std::end(m_MergeFontStack)); }
    }

    void ImGuiRenderer::RenderDrawData(const ImDrawData& drawData)
    {
        ZoneScopedN("ImGuiRenderer::RenderDrawData");// NOLINT

        m_VertexArray->Reset();

        m_PreviousRendererState = JE_APP.RendererAPI().RendererState();

        SetupRenderState(drawData);

        for (int i = 0; i < drawData.CmdListsCount; ++i) {
            const auto& commandList = *drawData.CmdLists[i];// NOLINT

            // Setup VAO stuff
            m_VertexBuffer->SetData({ reinterpret_cast<const std::byte*>(commandList.VtxBuffer.Data),// NOLINT
                                      static_cast<size_t>(commandList.VtxBuffer.Size) * sizeof(ImDrawVert) });
            m_IndexBuffer->SetData({ reinterpret_cast<const uint32_t*>(commandList.IdxBuffer.Data),// NOLINT
                                     static_cast<size_t>(commandList.IdxBuffer.Size) });

            RenderCommandList(commandList, drawData);
        }

        m_Shader->Unbind();

        JE_APP.RendererAPI().SetRendererState(m_PreviousRendererState);

        m_VertexArray->Delete();
    }

    void ImGuiRenderer::AddMergeFont(ImFontAtlas& fontAtlas)
    {
        if (!m_MergeFontStack.empty()) {
            for (auto font = std::rbegin(m_MergeFontStack); font != std::rend(m_MergeFontStack); ++font) {
                auto&        mergeFont = *font;
                ImFontConfig mergeFontConfig;
                mergeFontConfig.MergeMode        = true;
                mergeFontConfig.GlyphMinAdvanceX = mergeFont.xAdvance;
                fontAtlas.AddFontFromFileTTF(
                mergeFont.FilePath.c_str(), mergeFont.PixelSize, &mergeFontConfig, mergeFont.GlyphRanges.data());

                if (!mergeFont.MergeWithPrevious) { break; }
            }
        } else {
            Logger::CoreLogger().error("AddMergeFont: No Merge font available");
        }
    }

    void ImGuiRenderer::SetupRenderState(const ImDrawData& drawData)
    {
        ZoneScopedN("ImGuiRenderer::SetupRenderState");// NOLINT
        JE_APP.RendererAPI().SetRendererState(m_ImGuiRendererState);

        m_Shader->Bind();
        m_Shader->SetMat4("u_OrthoProjection",
                          glm::ortho(drawData.DisplayPos.x,
                                     drawData.DisplayPos.x + drawData.DisplaySize.x,
                                     drawData.DisplayPos.y + drawData.DisplaySize.y,
                                     drawData.DisplayPos.y));
        m_Shader->SetInt("u_Texture", 0);
    }

    void ImGuiRenderer::RenderCommandList(const ImDrawList& drawList, const ImDrawData& drawData)
    {
        for (int i = 0; i < drawList.CmdBuffer.Size; ++i) {
            const auto& command = drawList.CmdBuffer[i];

            if (command.UserCallback != nullptr) {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset
                // render state.)
                if (command.UserCallback == ImDrawCallback_ResetRenderState)// NOLINT
                {
                    SetupRenderState(drawData);
                } else {
                    command.UserCallback(&drawList, &command);
                }
            }

            // Project scissor/clipping rectangles into framebuffer space
            ImVec2 clip_min((command.ClipRect.x - drawData.DisplayPos.x) * drawData.FramebufferScale.x,
                            (command.ClipRect.y - drawData.DisplayPos.y) * drawData.FramebufferScale.y);
            ImVec2 clip_max((command.ClipRect.z - drawData.DisplayPos.x) * drawData.FramebufferScale.x,
                            (command.ClipRect.w - drawData.DisplayPos.y) * drawData.FramebufferScale.y);
            if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y) { continue; }

            JE_APP.RendererAPI().SetClipRect(
            { { static_cast<int>(clip_min.x), static_cast<int>((drawData.DisplaySize.y * drawData.FramebufferScale.y - clip_max.y)) },
              { static_cast<int>((clip_max.x - clip_min.x)), static_cast<int>((clip_max.y - clip_min.y)) } });

            // Bind Texture
            auto rendererID = command.GetTexID();// NOLINT
            JE_APP.RendererAPI().BindTexture(rendererID);

            // DrawCall
            JE_APP.RendererAPI().DrawIndexed(*m_VertexArray, command.ElemCount, command.IdxOffset);
        }
    }

}// namespace JE