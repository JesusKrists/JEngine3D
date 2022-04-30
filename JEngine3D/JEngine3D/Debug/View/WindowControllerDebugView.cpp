#include "WindowControllerDebugView.hpp"

#include "JEngine3D/Core/WindowController.hpp"
#include "JEngine3D/Platform/IGraphicsContext.hpp"

#include <imgui.h>

#include <imgui_internal.h>
#include <string>

namespace JE {

static constexpr auto WINDOW_PARAMETER_COLOR = ImVec4{ 1, 1, 0, 1 };
static constexpr auto WINDOW_PARAMETER_ALIGNMENT_START = 128;


void WindowControllerDebugView::OnImGuiRender()
{
  if (ImGui::Begin(Name().c_str(), &m_Open)) {

    auto RenderWindowParameter = [](const std::string &label,
                                   const std::string &parameter,
                                   const ImVec4 &parameterColor = WINDOW_PARAMETER_COLOR) {
      auto labelSize = ImGui::CalcTextSize(label.c_str());

      ImGui::TextUnformatted(label.c_str());
      ImGui::SameLine(0, WINDOW_PARAMETER_ALIGNMENT_START - labelSize.x);
      ImGui::TextColored(parameterColor, "%s", parameter.c_str());// NOLINT
    };

    const auto &windows = WindowController::Get().Windows();

    static Window *s_SelectedWindow = nullptr;// NOLINT
    if (FindIf(windows, [](auto &window) { return window.get() == s_SelectedWindow; }) == std::end(windows)) {
      s_SelectedWindow = nullptr;
    }

    ImGui::BeginChild("WindowList", { 200, 0 }, true);// NOLINT
    ForEach(windows, [](const Scope<Window, MemoryTag::App> &window) {
      if (ImGui::Selectable(window->Title().c_str(), s_SelectedWindow == window.get())) {
        s_SelectedWindow = window.get();
      }
    });
    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::BeginGroup();
    if (s_SelectedWindow != nullptr) {
      ImGui::TextUnformatted("Window Parameters");
      ImGui::Indent();

      RenderWindowParameter("Title:", s_SelectedWindow->Title(), { 1, 1, 1, 1 });
      RenderWindowParameter("Size:", fmt::format("{}", s_SelectedWindow->Size()));
      RenderWindowParameter("Drawable Size:", fmt::format("{}", s_SelectedWindow->GraphicsContext().DrawableSize()));
      RenderWindowParameter("Position:", fmt::format("{}", s_SelectedWindow->Position()));
      RenderWindowParameter("Visible:", fmt::format("{}", s_SelectedWindow->Shown()));
      RenderWindowParameter("Focused:", fmt::format("{}", s_SelectedWindow->Focused()));
      RenderWindowParameter("Minimized:", fmt::format("{}", s_SelectedWindow->Minimized()));

      ImGui::Unindent();
    }
    ImGui::EndGroup();
  }
  ImGui::End();
}


}// namespace JE