#include "WindowControllerDebugView.hpp"

#include "JEngine3D/Core/WindowController.hpp"
#include "JEngine3D/Platform/IGraphicsContext.hpp"

#include <string>
#include <iterator>// for end

#include <fmt/core.h>// for format
#include <imgui.h>

namespace JE {

    static constexpr auto WINDOW_PARAMETER_ALIGNMENT_START = 128;


    void WindowControllerDebugView::OnImGuiRender()
    {
        auto RenderWindowParameter =
        [](const std::string_view& label, const std::string& parameter, const Color& parameterColor = PARAMETER_COLOR) {
            auto labelSize = ImGui::CalcTextSize(label.data());

            ImGui::TextUnformatted(label.data());
            ImGui::SameLine(0, WINDOW_PARAMETER_ALIGNMENT_START - labelSize.x);
            ImGui::TextColored(parameterColor, "%s", parameter.c_str());// NOLINT
        };

        const auto& windows = WindowController::Get().Windows();

        static Window* s_SelectedWindow = nullptr;// NOLINT
        if (FindIf(windows, [](auto& window) { return window.get() == s_SelectedWindow; }) == std::end(windows)) {
            s_SelectedWindow = nullptr;
        }

        ImGui::BeginChild("WindowList", { 200, 0 }, true);// NOLINT
        ForEach(windows, [](const Scope<Window, MemoryTag::App>& window) {
            const auto* title = window->Title().c_str();
            if (ImGui::Selectable(title[0] == '\0' ? "Unknown Window" : title, s_SelectedWindow == window.get())) {// NOLINT
                s_SelectedWindow = window.get();
            }
        });
        ImGui::EndChild();
        ImGui::SameLine();

        ImGui::BeginGroup();
        if (s_SelectedWindow != nullptr) {
            ImGui::TextUnformatted("Window Properties");
            ImGui::Indent();

            RenderWindowParameter("Title:", s_SelectedWindow->Title(), { 1, 1, 1, 1 });
            RenderWindowParameter("Size:", fmt::format("{}", s_SelectedWindow->Size()));
            RenderWindowParameter("Drawable Size:", fmt::format("{}", s_SelectedWindow->GraphicsContext().DrawableSize()));
            RenderWindowParameter("Position:", fmt::format("{}", s_SelectedWindow->Position()));
            RenderWindowParameter(
            "Visible:", fmt::format("{}", s_SelectedWindow->Shown()), s_SelectedWindow->Shown() ? TRUE_COLOR : FALSE_COLOR);
            RenderWindowParameter(
            "Focused:", fmt::format("{}", s_SelectedWindow->Focused()), s_SelectedWindow->Focused() ? TRUE_COLOR : FALSE_COLOR);
            RenderWindowParameter(
            "Minimized:", fmt::format("{}", s_SelectedWindow->Minimized()), s_SelectedWindow->Minimized() ? TRUE_COLOR : FALSE_COLOR);

            ImGui::Unindent();
        }
        ImGui::EndGroup();
    }


}// namespace JE