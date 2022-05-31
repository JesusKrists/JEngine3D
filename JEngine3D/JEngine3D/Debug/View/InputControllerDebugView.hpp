#pragma once

#include "JEngine3D/Debug/View/IImGuiDebugView.hpp"

namespace JE {

class InputControllerDebugView : public IImGuiDebugView
{
public:
  InputControllerDebugView() : IImGuiDebugView("InputControllerDebugView") {}

private:
  void OnImGuiRender() override;
};

}// namespace JE