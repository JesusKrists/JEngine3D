#pragma once

#include "JEngine3D/Debug/View/IImGuiDebugView.hpp"


namespace JE {

class WindowControllerDebugView final : public IImGuiDebugView
{
public:
  WindowControllerDebugView() : IImGuiDebugView("WindowControllerDebugView") {}
  void OnImGuiRender() override;
};

}// namespace JE