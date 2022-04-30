#include "ApplicationDebugView.hpp"
#include "JEngine3D/Core/Application.hpp"


#include <fmt/core.h>
#include <imgui.h>

namespace JE {


static constexpr auto APPLICATION_PARAMETER_COLOR = ImVec4{ 1, 0, 1, 1 };
static constexpr auto APPLICATION_PARAMETER_ALIGNMENT_START = 128;

void ApplicationDebugView::OnImGuiRender()
{
  if (ImGui::Begin(Name().c_str(), &m_Open)) {

    auto RenderApplicationParameter = [](const std::string &label,
                                        const std::string &parameter,
                                        const ImVec4 &parameterColor = APPLICATION_PARAMETER_COLOR) {
      auto labelSize = ImGui::CalcTextSize(label.c_str());

      ImGui::TextUnformatted(label.c_str());
      ImGui::SameLine(0, APPLICATION_PARAMETER_ALIGNMENT_START - labelSize.x);
      ImGui::TextColored(parameterColor, "%s", parameter.c_str());// NOLINT
    };

    auto &app = JE::Application::Get();


    ImGui::TextUnformatted("Application Parameters");
    ImGui::Indent();

    ImGui::BeginGroup();
    RenderApplicationParameter("Running:", fmt::format("{}", app.Running()));
    RenderApplicationParameter("Delta Time:", fmt::format("{}", app.DeltaTime()));
    RenderApplicationParameter("Focused:", fmt::format("{}", app.Focused()));
    RenderApplicationParameter("Total Frame Count:", fmt::format("{}", app.TotalFrameCount()));
    ImGui::EndGroup();

    ImGui::Unindent();

    ImGui::TextUnformatted("Layers");
    ImGui::Indent();

    ImGui::BeginChild("LayerList");
    ForEach(app.Layers(), [](const ILayer &layer) { ImGui::TextUnformatted(layer.DebugName().c_str()); });
    ImGui::EndChild();

    ImGui::Unindent();
  }
  ImGui::End();
}

}// namespace JE