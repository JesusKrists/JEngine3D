#include "InputControllerDebugView.hpp"

#include "JEngine3D/Core/InputController.hpp"

#include <string>// for string, allocator
#include <unordered_map>// for operator==, unordered_map

#include <fmt/core.h>// for format
#include <imgui.h>


namespace JE {


static constexpr auto INPUTCONTROLLER_PARAMETER_ALIGNMENT_START = 150;
static constexpr auto INPUTCONTROLLER_MAP_ALIGNMENT_START = 128;

void InputControllerDebugView::OnImGuiRender()
{

  auto RenderInputControllerParameter =
    [](const std::string &label, const std::string &parameter, const Color &parameterColor = PARAMETER_COLOR) {
      auto labelSize = ImGui::CalcTextSize(label.c_str());

      ImGui::TextUnformatted(label.c_str());
      ImGui::SameLine(0, INPUTCONTROLLER_PARAMETER_ALIGNMENT_START - labelSize.x);
      ImGui::TextColored(parameterColor, "%s", parameter.c_str());// NOLINT
    };

  auto RenderInputControllerMap = [](auto &map) {
    UNUSED(map);

    for (const auto &[key, value] : map) {
      const auto *keyStr = JEngine3DKeyCodeToString(key);

      auto labelSize = ImGui::CalcTextSize(keyStr);
      ImGui::TextUnformatted(keyStr);
      ImGui::SameLine(0, INPUTCONTROLLER_MAP_ALIGNMENT_START - labelSize.x);
      ImGui::TextColored(value ? TRUE_COLOR : FALSE_COLOR, "%s", fmt::format("{}", value).c_str());// NOLINT
    }
  };

  const auto &inputController = InputController::Get();


  ImGui::TextUnformatted("InputController Properties");
  ImGui::Indent();

  ImGui::BeginGroup();
  RenderInputControllerParameter("Mouse Position:", fmt::format("{}", inputController.MousePosition()));
  RenderInputControllerParameter(
    "Relative Mouse Position:", fmt::format("{}", inputController.RelativeMousePosition()));
  RenderInputControllerParameter("Mouse Scroll Amount:", fmt::format("{}", inputController.MouseScrollAmount()));
  ImGui::EndGroup();

  ImGui::Dummy({ 0, 16 });// NOLINT
  ImGui::Unindent();
  ImGui::Separator();
  ImGui::TextUnformatted("Key and Mouse button states");
  ImGui::Indent();

  auto availableSpace = ImGui::GetContentRegionAvail();
  ImGui::BeginChild("KeyButtonStates", { availableSpace.x / 2, 0 });
  RenderInputControllerMap(inputController.KeyMap());
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::BeginChild("MouseButtonStates", { availableSpace.x / 2, 0 });
  RenderInputControllerMap(inputController.MouseMap());
  ImGui::EndChild();

  ImGui::Unindent();
}

}// namespace JE