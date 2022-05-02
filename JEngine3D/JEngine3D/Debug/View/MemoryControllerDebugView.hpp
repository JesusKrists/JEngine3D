#pragma once

#include "JEngine3D/Debug/View/IImGuiDebugView.hpp"

namespace JE {

class MemoryControllerDebugView final : public IImGuiDebugView
{
public:
  MemoryControllerDebugView() : IImGuiDebugView("MemoryControllerDebugView") {}

private:
  void OnImGuiRender() override;
};

}// namespace JE