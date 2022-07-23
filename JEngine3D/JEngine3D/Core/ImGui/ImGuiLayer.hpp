#pragma once

#include "JEngine3D/Core/ILayer.hpp"
#include "JEngine3D/Renderer/ImGui/ImGuiRenderer.hpp"

namespace JE {


    class IEvent;

    class ImGuiLayer final : public ILayer
    {
    public:
        ImGuiLayer() : ILayer("JEngine ImGuiLayer") {}

        void OnCreate() override;
        void OnDestroy() override;

        void OnUpdate() override;
        void OnImGuiRender() override;

        void OnEvent(JE::IEvent& event) override;

        void                      SetCaptureEvents(bool capture = true);
        [[nodiscard]] inline auto CaptureEvents() const -> bool { return m_CaptureEvents; }

        // cppcheck-suppress functionStatic
        void Begin();
        // cppcheck-suppress functionStatic
        void End();

        // cppcheck-suppress functionConst
        [[nodiscard]] inline auto Renderer() -> ImGuiRenderer& { return m_ImGuiRenderer; }

    private:
        ImGuiRenderer m_ImGuiRenderer;

        bool m_CaptureEvents = true;
    };

}// namespace JE