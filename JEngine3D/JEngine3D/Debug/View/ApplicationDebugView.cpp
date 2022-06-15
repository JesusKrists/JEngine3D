#include "ApplicationDebugView.hpp"
#include "JEngine3D/Core/Application.hpp"

#include <string>// for string, allocator
#include "JEngine3D/Core/Base.hpp"// for ForEach
#include "JEngine3D/Core/ILayer.hpp"// for ILayer
#include "JEngine3D/Core/ImGui/ImGuiLayer.hpp"// for ImGuiLayer

#include <fmt/core.h>
#include <imgui.h>

namespace JE {

static constexpr auto MILISECONDS = 1000;

static constexpr auto APPLICATION_PARAMETER_ALIGNMENT_START = 150;

void ApplicationDebugView::OnImGuiRender()
{
  auto RenderApplicationParameter =
    [](const std::string &label, const std::string &parameter, const ImVec4 &parameterColor = PARAMETER_COLOR) {
      auto labelSize = ImGui::CalcTextSize(label.c_str());

      ImGui::TextUnformatted(label.c_str());
      ImGui::SameLine(0, APPLICATION_PARAMETER_ALIGNMENT_START - labelSize.x);
      ImGui::TextColored(parameterColor, "%s", parameter.c_str());// NOLINT
    };

  auto &app = JE_APP;


  ImGui::TextUnformatted("Application Properties");
  ImGui::Indent();

  ImGui::BeginGroup();
  RenderApplicationParameter("Running:", fmt::format("{}", app.Running()), app.Running() ? TRUE_COLOR : FALSE_COLOR);
  RenderApplicationParameter("Focused:", fmt::format("{}", app.Focused()), app.Focused() ? TRUE_COLOR : FALSE_COLOR);
  RenderApplicationParameter("Delta Time:", fmt::format("{}ms", app.DeltaTime() * MILISECONDS));
  RenderApplicationParameter("Total Frame Count:", fmt::format("{}", app.TotalFrameCount()));
  RenderApplicationParameter("ImGui Capturing Events:",
    fmt::format("{}", app.ImGuiLayer().CaptureEvents()),
    app.ImGuiLayer().CaptureEvents() ? TRUE_COLOR : FALSE_COLOR);
  ImGui::EndGroup();
  ImGui::Dummy({ 0, 16 });// NOLINT
  ImGui::Unindent();

  ImGui::Separator();
  ImGui::TextUnformatted("Layers");
  ImGui::Indent();

  ImGui::BeginChild("LayerList");
  ForEach(app.Layers(),
    [](const Scope<ILayer, MemoryTag::App> &layer) { ImGui::TextUnformatted(layer->DebugName().c_str()); });
  ImGui::EndChild();

  ImGui::Unindent();
}

}// namespace JE