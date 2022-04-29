#pragma once

#include "JEngine3D/Debug/View/IImGuiDebugView.hpp"


namespace JE {

class WindowControllerDebugView : public IImGuiDebugView
{
public:
  WindowControllerDebugView() : IImGuiDebugView("WindowControllerDebugView") {}
  void OnImGuiRender() override;
};

}// namespace JE