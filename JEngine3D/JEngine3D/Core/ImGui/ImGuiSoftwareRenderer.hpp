#pragma once


namespace JE {

class IGraphicsContext;

class ImGuiSoftwareRenderer
{
public:
  static void Initialize();

  static void RenderImGui(IGraphicsContext &context);

  static void Destroy();
};

}// namespace JE