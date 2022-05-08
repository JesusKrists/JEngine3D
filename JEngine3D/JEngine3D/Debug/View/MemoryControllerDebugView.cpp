#include "MemoryControllerDebugView.hpp"

#include "JEngine3D/Core/MemoryController.hpp"

#include <string>// for string, allocator

#include <fmt/core.h>
#include <imgui.h>

namespace JE {

static constexpr auto MEMORYCONTROLLER_PARAMETER_ALIGNMENT_START = 150;

void MemoryControllerDebugView::OnImGuiRender()
{
  auto RenderMemoryControllerParameter =
    [](const std::string &label, const std::string &parameter, const ImVec4 &parameterColor = PARAMETER_COLOR) {
      auto labelSize = ImGui::CalcTextSize(label.c_str());

      ImGui::TextUnformatted(label.c_str());
      ImGui::SameLine(0, MEMORYCONTROLLER_PARAMETER_ALIGNMENT_START - labelSize.x);
      ImGui::TextColored(parameterColor, "%s", parameter.c_str());// NOLINT
    };

  const auto &memoryController = JE::MemoryController::Get();


  ImGui::TextUnformatted("MemoryController Properties");
  ImGui::Indent();

  ImGui::BeginGroup();
  RenderMemoryControllerParameter("Total Allocations:", fmt::format("{}", memoryController.TotalAllocCount()));
  RenderMemoryControllerParameter("Total Deallocations:", fmt::format("{}", memoryController.TotalDeallocCount()));
  RenderMemoryControllerParameter("Alloc/Dealloc Delta:",
    fmt::format("{}", memoryController.TotalAllocCount() - memoryController.TotalDeallocCount()));
  ImGui::EndGroup();

  ImGui::Unindent();
}

}// namespace JE