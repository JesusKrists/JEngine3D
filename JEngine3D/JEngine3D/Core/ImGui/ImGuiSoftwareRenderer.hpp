#pragma once


class ImDrawData;
namespace JE {

class Window;

class ImGuiSoftwareRenderer
{
public:
  static void Initialize();

  static void RenderImGui(Window &window, ImDrawData *drawData);

  static void Destroy();
};

}// namespace JE