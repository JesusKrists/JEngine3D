#include "UILayer.hpp"
#include "UI/ContentBrowserPanel.hpp"
#include "UI/IPanel.hpp"
#include "JEditorState.hpp"

#include <JEngine3D/Core/Application.hpp>
#include <JEngine3D/Debug/View/IImGuiDebugView.hpp>
#include <JEngine3D/Core/ImGui/ImGuiLayer.hpp>
#include <JEngine3D/Renderer/IFramebuffer.hpp>
#include <JEngine3D/Renderer/ITexture.hpp>
#include <JEngine3D/Renderer/Renderer2D.hpp>
#include <JEngine3D/Renderer/IRendererObjectCreator.hpp>
#include <JEngine3D/Utility/ImageLoader.hpp>

#include <imgui.h>
#include <IconsFontAwesome6.h>

#include <filesystem>

#include <Tracy.hpp>

// IWYU pragma: no_include <glm/detail/type_vec3.inl>
// IWYU pragma: no_include <glm/detail/type_vec4.inl>
// IWYU pragma: no_include <glm/ext/vector_float3.hpp>


#include <entt/entt.hpp>
#include <JEngine3D/ECS/Entity.hpp>
#include <JEngine3D/ECS/Components.hpp>

namespace JE {
    class IEvent;
}

namespace JEditor {

    static constexpr auto GAME_VIEWPORT_OUTLINE_COLOR = IM_COL32(255, 255, 255, 127);// NOLINT
    static constexpr auto ICON_IMAGE_SIZE             = JE::Size2DI{ 128, 128 };

    void UILayer::OnCreate()
    {
        ZoneScopedN("UILayer::OnCreate");// NOLINT

        constexpr uint32_t WHITE_COLOR = 0xFFFFFFFF;
        m_TestTexture                  = JE::IRendererObjectCreator::Get().CreateTexture();
        m_TestTexture->SetData("UILayer test texture",
                               { reinterpret_cast<const std::byte*>(&WHITE_COLOR), 4 },// NOLINT
                               { 1, 1 },
                               JE::TextureFormat::RGBA8);


        m_MemeTexture = JE::AssetController::Get().LoadAsset("assets/textures/testtexture.jpg");

        LoadImGuiSettings();

        JE::ForEach(JE_APP.DebugViews(), [](JE::IImGuiDebugView& view) { view.Open(); });
        InitializeUI();

        entt::registry testRegistry;
        JE::Entity     testEntity{ testRegistry.create(), testRegistry };
        auto&          testComponent = testEntity.AddComponent<JE::TagComponent>();
        testComponent.Tag            = "Tag Component String Value";

        auto view = testRegistry.view<const JE::TagComponent>();
        JE::ForEach(view, [&](entt::entity entity) {
            auto [tagComponent] = view.get(entity);
            JE::Logger::ClientLogger().debug("{}", tagComponent.Tag);
        });

        auto& component = testEntity.GetComponent<JE::TagComponent>();
        component.Tag   = "New Tag Component String Value";

        auto view2 = testRegistry.view<const JE::TagComponent>();
        JE::ForEach(view2, [&](entt::entity entity) {
            auto [tagComponent] = view.get(entity);
            JE::Logger::ClientLogger().debug("{}", tagComponent.Tag);
        });


        JE::Logger::ClientLogger().debug("Test Entity has component - {}", testEntity.HasComponent<JE::TagComponent>());

        testEntity.DeleteComponent<JE::TagComponent>();

        JE::Logger::ClientLogger().debug("Test Entity has component - {}", testEntity.HasComponent<JE::TagComponent>());
    }

    void UILayer::OnDestroy() {}

    void UILayer::OnUpdate()
    {
        ZoneScopedN("UILayer::OnUpdate");// NOLINT
        auto UpdateImGuiLayer = [&]() {
            if (m_ResetDockLayout) {
                ImGui::LoadIniSettingsFromDisk("assets/EditorUI/imgui/default_layout.ini");
                m_ResetDockLayout = false;
            }

            if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Escape)) && !JE_APP.ImGuiLayer().CaptureEvents()) {
                JE_APP.ImGuiLayer().SetCaptureEvents(true);
            }

            if (EditorState::Get().GameViewportFBO->Configuration().Size != m_GameViewportSize) {
                EditorState::Get().GameViewportFBO->Resize(m_GameViewportSize);
            }
        };
        UpdateImGuiLayer();

        auto Renderer2DTest = [&]() {
            ZoneScopedN("Renderer2D Test");// NOLINT

            static constexpr auto CLEAR_COLOR = JE::Color{ 0.1F, 0.1F, 0.1F, 1.0F };

            auto& rendererAPI = JE_APP.RendererAPI();
            auto& renderer2D  = JE_APP.Renderer2D();

            rendererAPI.SetClearColor(CLEAR_COLOR);
            rendererAPI.Clear();

            EditorState::Get().GameViewportFBO->Bind();
            rendererAPI.Clear();
            EditorState::Get().GameViewportFBO->Unbind();


            renderer2D.BeginBatch(EditorState::Get().GameViewportFBO.get());

            auto vertex0 = JE::Vertex{ glm::vec3{ -0.5F, 0.0F, 0.0F }, JE::Color{ 1.0F, 0.0F, 0.0F, 1.0F } };// NOLINT
            auto vertex1 = JE::Vertex{ glm::vec3{ 0.5F, 0.0F, 0.0F }, JE::Color{ 0.0F, 1.0F, 0.0F, 1.0F } };// NOLINT
            auto vertex2 = JE::Vertex{ glm::vec3{ 0.0F, 1.0F, 0.0F }, JE::Color{ 0.0F, 0.0F, 1.0F, 1.0F } };// NOLINT

            auto vertex3 = JE::Vertex{ glm::vec3{ -0.5F, -1.0F, 0.0F }, JE::Color{ 1.0F, 0.0F, 0.0F, 1.0F } };// NOLINT
            auto vertex4 = JE::Vertex{ glm::vec3{ 0.5F, -1.0F, 0.0F }, JE::Color{ 0.0F, 1.0F, 0.0F, 1.0F } };// NOLINT
            auto vertex5 = JE::Vertex{ glm::vec3{ 0.0F, 0.0F, 0.0F }, JE::Color{ 0.0F, 0.0F, 1.0F, 1.0F } };// NOLINT

            constexpr auto position = glm::vec3{ -0.80F, -0.80F, 0.0F };
            constexpr auto size     = glm::vec2{ 0.15F, 0.15F };
            constexpr auto color    = JE::Color{ 1.0F, 0.0F, 1.0F, 1.0F };

            // constexpr auto position2 = glm::vec3{ 0.0F, 0.0F, 0.0F };
            constexpr auto color2 = JE::Color{ 1.0F, 1.0F, 1.0F, 1.0F };

            renderer2D.DrawTriangle(vertex0, vertex1, vertex2, *m_TestTexture);
            renderer2D.DrawTriangle(vertex3, vertex4, vertex5);
            renderer2D.DrawQuad(position, size, color);
            for (int y = 0; y < 10; y++) {// NOLINT
                for (int x = 0; x < 10; x++) {// NOLINT
                    const auto newPosition = glm::vec3{ -0.9F + (static_cast<float>(x) / 5), -0.9F + (static_cast<float>(y) / 5), 0 };
                    renderer2D.DrawQuad(newPosition, size, m_MemeTexture->As<JE::ITexture2D>(), color2);
                }
            }

            renderer2D.EndBatch();
        };

        Renderer2DTest();
    }

    void UILayer::OnImGuiRender()
    {
        ZoneScopedN("UILayer::OnImGuiRender");// NOLINT
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        RenderMainMenuBar();
        RenderGameViewport();
        ForEach(m_UIPanels, [](auto& view) {
            if (view->IsOpen()) { view->Render(); }
        });

        ImGui::ShowDemoWindow();
    }

    void UILayer::OnEvent(JE::IEvent& event) { JE::UNUSED(event); }

    void UILayer::InitializeUI()
    {
        auto& imguiIO = ImGui::GetIO();

        static constexpr std::array<ImWchar, 2> icon_ranges = { ICON_MIN_FA, ICON_MAX_FA };// NOLINT
        JE_APP.ImGuiLayer().PushMergeFont("assets/EditorUI/fonts/" FONT_ICON_FILE_NAME_FAS, 16.0f, 16.0f, icon_ranges);// NOLINT
        JE_APP.ImGuiLayer().PushMergeFont("assets/EditorUI/fonts/" FONT_ICON_FILE_NAME_FAR, 16.0f, 16.0f, icon_ranges, true);// NOLINT

        // Load default font
        auto* font          = JE_APP.ImGuiLayer().AddFont("assets/EditorUI/fonts/SEGOEUI.TTF", 16.0f);// NOLINT
        imguiIO.FontDefault = font;

        EditorState::Get().Segoe16Bold = JE_APP.ImGuiLayer().AddFont("assets/EditorUI/fonts/SegoeBold.ttf", 16.0f);// NOLINT
        EditorState::Get().Segoe24Bold = JE_APP.ImGuiLayer().AddFont("assets/EditorUI/fonts/SegoeBold.ttf", 24.0f);// NOLINT

        if (!EditorState::Get().GameViewportFBO) {
            EditorState::Get().GameViewportFBO = JE::IRendererObjectCreator::Get().CreateFramebuffer(
            { m_GameViewportSize, { JE::FramebufferAttachmentFormat::RGBA8, JE::FramebufferAttachmentFormat::DEPTH24STENCIL8 } });
        }

        BuildIconMap();

        m_UIPanels.push_back(JE::CreatePolymorphicScope<ContentBrowserPanel, JE::MemoryTag::Editor, IPanel>());
    }

    void UILayer::BuildIconMap()// NOLINT
    {
        if (EditorState::Get().FileIconMap.empty()) {
            auto folderImage = JE::ImageLoader::LoadImageFromPath(
            JE_APP.WORKING_DIRECTORY + "/" + "assets/EditorUI/textures/icons/folder.svg", JE::ImageConfig{ ICON_IMAGE_SIZE });

            auto folderOpenImage = JE::ImageLoader::LoadImageFromPath(
            JE_APP.WORKING_DIRECTORY + "/" + "assets/EditorUI/textures/icons/folder-open.svg", JE::ImageConfig{ ICON_IMAGE_SIZE });

            auto svgImage = JE::ImageLoader::LoadImageFromPath(JE_APP.WORKING_DIRECTORY + "/" + "assets/EditorUI/textures/icons/svgo.svg",
                                                               JE::ImageConfig{ ICON_IMAGE_SIZE });

            auto imageImage = JE::ImageLoader::LoadImageFromPath(
            JE_APP.WORKING_DIRECTORY + "/" + "assets/EditorUI/textures/icons/image.svg", JE::ImageConfig{ ICON_IMAGE_SIZE });

            auto fontImage = JE::ImageLoader::LoadImageFromPath(JE_APP.WORKING_DIRECTORY + "/" + "assets/EditorUI/textures/icons/font.svg",
                                                                JE::ImageConfig{ ICON_IMAGE_SIZE });

            auto iniImage = JE::ImageLoader::LoadImageFromPath(
            JE_APP.WORKING_DIRECTORY + "/" + "assets/EditorUI/textures/icons/settings.svg", JE::ImageConfig{ ICON_IMAGE_SIZE });

            auto unknownFileImage = JE::ImageLoader::LoadImageFromPath(
            JE_APP.WORKING_DIRECTORY + "/" + "assets/EditorUI/textures/icons/file.svg", JE::ImageConfig{ ICON_IMAGE_SIZE });


            EditorState::Get().FileIconMap[FileExtension::FOLDER]      = JE::IRendererObjectCreator::Get().CreateTexture(folderImage);
            EditorState::Get().FileIconMap[FileExtension::FOLDER_OPEN] = JE::IRendererObjectCreator::Get().CreateTexture(folderOpenImage);
            EditorState::Get().FileIconMap[FileExtension::SVG]         = JE::IRendererObjectCreator::Get().CreateTexture(svgImage);
            EditorState::Get().FileIconMap[FileExtension::JPG]         = JE::IRendererObjectCreator::Get().CreateTexture(imageImage);
            EditorState::Get().FileIconMap[FileExtension::TTF]         = JE::IRendererObjectCreator::Get().CreateTexture(fontImage);
            EditorState::Get().FileIconMap[FileExtension::INI]         = JE::IRendererObjectCreator::Get().CreateTexture(iniImage);
            EditorState::Get().FileIconMap[FileExtension::UNKNOWN]     = JE::IRendererObjectCreator::Get().CreateTexture(unknownFileImage);
        }
    }

    void UILayer::LoadImGuiSettings()// NOLINT(readability-convert-member-functions-to-static)
    {
        if (!std::filesystem::exists("imgui.ini")) { ImGui::LoadIniSettingsFromDisk("assets/EditorUI/imgui/default_layout.ini"); }
    }

    // NOLINTNEXTLINE
    void UILayer::RenderMainMenuBar()
    {
        ZoneScopedN("UILayer::RenderMainMenuBar");// NOLINT
        auto RenderMenuBarViews = [&]() {
            JE::ForEach(m_UIPanels, [](auto& panel) {
                if (ImGui::MenuItem(panel->Name().c_str(), nullptr, panel->IsOpen())) {
                    if (panel->IsOpen()) {
                        panel->Close();
                    } else {
                        panel->Open();
                    }
                }
            });
        };

        auto RenderMenuBarDebugViews = [&]() {
            auto& views = JE_APP.DebugViews();

            JE::ForEach(views, [](JE::IImGuiDebugView& view) {
                if (ImGui::MenuItem(view.Name().c_str(), nullptr, view.IsOpen())) {
                    if (view.IsOpen()) {
                        view.Close();
                    } else {
                        view.Open();
                    }
                }
            });
        };

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                if (ImGui::MenuItem("Reset Dock Layout")) { m_ResetDockLayout = true; }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Views")) {
                RenderMenuBarViews();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Debug")) {
                if (ImGui::BeginMenu("Views")) {
                    RenderMenuBarDebugViews();
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void UILayer::RenderGameViewport()
    {
        ZoneScopedN("UILayer::RenderGameViewport");// NOLINT
        ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
        ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, 0);
        ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 2, 2 });
        ImGui::Begin("Game Viewport", nullptr);
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("GameRender");
            // Get the size of the child (i.e. the whole draw size of the windows).
            ImVec2 size                = ImGui::GetWindowSize();
            ImVec2 absoluteCursorPos   = ImGui::GetCursorScreenPos();
            ImVec2 absoluteCursorStart = { absoluteCursorPos.x - 1, absoluteCursorPos.y - 1 };
            ImVec2 absoluteCursorEnd   = { absoluteCursorPos.x + size.x + 1, absoluteCursorPos.y + size.y + 1 };

            m_GameViewportSize = { static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) };

            ImGui::Image(EditorState::Get().GameViewportFBO->AttachmentRendererID(),
                         EditorState::Get().GameViewportFBO->Configuration().Size,
                         { 0, 1 },
                         { 1, 0 });

            if (!JE_APP.ImGuiLayer().CaptureEvents()) {
                ImGui::PushClipRect(absoluteCursorStart, absoluteCursorEnd, false);
                ImGui::GetWindowDrawList()->AddRect(absoluteCursorStart, absoluteCursorEnd, GAME_VIEWPORT_OUTLINE_COLOR);// NOLINT
                ImGui::PopClipRect();
            }

            if (ImGui::IsItemClicked()) { JE_APP.ImGuiLayer().SetCaptureEvents(false); }


            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
    }

}// namespace JEditor