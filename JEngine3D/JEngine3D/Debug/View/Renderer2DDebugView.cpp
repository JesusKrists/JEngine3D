#include "Renderer2DDebugView.hpp"
#include "JEngine3D/Renderer/Renderer2D.hpp"
#include "JEngine3D/Core/Application.hpp"

#include <string>// for string, allocator
#include <cstddef>// for size_t

#include <fmt/core.h>
#include <imgui.h>

namespace JE {

// static constexpr auto MILISECONDS = 1000;

static constexpr auto RENDERER_PARAMETER_ALIGNMENT_START = 128;

void Renderer2DDebugView::OnImGuiRender()
{
  auto RenderRenderer2DModifiableIntParameter =
    [](const std::string &label, size_t parameterValue, size_t min, size_t max, auto setterFunc) {// NOLINT
      int newValue = static_cast<int>(parameterValue);
      ImGui::SliderInt(label.c_str(), &newValue, static_cast<int>(min), static_cast<int>(max));
      if (static_cast<size_t>(newValue) != parameterValue) { setterFunc(static_cast<size_t>(newValue)); }
    };

  auto RenderRenderer2DParameter =
    [](const std::string &label, const std::string &parameter, const ImVec4 &parameterColor = PARAMETER_COLOR) {
      auto labelSize = ImGui::CalcTextSize(label.c_str());

      ImGui::TextUnformatted(label.c_str());
      ImGui::SameLine(0, RENDERER_PARAMETER_ALIGNMENT_START - labelSize.x);
      ImGui::TextColored(parameterColor, "%s", parameter.c_str());// NOLINT
    };

  auto &renderer2D = JE::Application::Get().Renderer2D();


  ImGui::TextUnformatted("Renderer2D Properties");
  ImGui::Indent();

  ImGui::BeginGroup();
  RenderRenderer2DModifiableIntParameter(
    "Quads Per Batch", renderer2D.QuadsPerBatch(), 1, Renderer2D::MAX_QUADS_PER_BATCH, [&](size_t batches) {
      renderer2D.SetQuadsPerBatch(batches);
    });
  RenderRenderer2DModifiableIntParameter(
    "Triangles Per Batch", renderer2D.TrianglesPerBatch(), 1, Renderer2D::MAX_TRIANGLES_PER_BATCH, [&](size_t batches) {
      renderer2D.SetTrianglesPerBatch(batches);
    });

  RenderRenderer2DParameter("Frame Quad Vertex Count:", fmt::format("{}", renderer2D.FrameQuadVertexCount()));
  RenderRenderer2DParameter("Frame Triangle Vertex Count:", fmt::format("{}", renderer2D.FrameTriangleVertexCount()));
  ImGui::EndGroup();

  ImGui::Unindent();
}

}// namespace JE