#include "WindowControllerDebugView.hpp"

#include <imgui.h>

#include <string>

namespace JE {

void WindowControllerDebugView::OnImGuiRender()
{
  ImGui::Begin(Name().c_str(), &m_Open);
  ImGui::End();
}

}// namespace JE