#pragma once

#include "JEngine3D/Core/ILayer.hpp"

namespace JE {


class IEvent;

class ImGuiLayer : public ILayer
{
public:
  ImGuiLayer() : ILayer("JEngine ImGuiLayer") {}

  void OnCreate() override;
  void OnDestroy() override;

  void OnUpdate() override;
  void OnImGuiRender() override;

  void OnEvent(JE::IEvent &event) override;
};

}// namespace JE