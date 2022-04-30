#pragma once

#include "JEngine3D/Debug/View/IImGuiDebugView.hpp"

namespace JE {

class ApplicationDebugView final : public IImGuiDebugView
{
public:
  ApplicationDebugView() : IImGuiDebugView("ApplicationDebugView") {}
  void OnImGuiRender() override;
};

}// namespace JE