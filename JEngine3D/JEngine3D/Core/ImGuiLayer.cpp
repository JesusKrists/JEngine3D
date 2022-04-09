#include "ImGuiLayer.hpp"

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT

#include <imgui.h>

namespace JE {

class IEvent;

static void InitializeImGuiForJEngine3D()
{
  const ImGuiIO &imguiIO = ImGui::GetIO();
  ASSERT(imguiIO.BackendPlatformUserData == nullptr, "Already initialized a platform backend!");

  // Setup backend capabilities flags
  /*ImGui_ImplSDL2_Data *bd = IM_NEW(ImGui_ImplSDL2_Data)();
  imguiIO.BackendPlatformUserData = (void *)bd;
  imguiIO.BackendPlatformName = "imgui_impl_sdl";
  imguiIO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;// We can honor GetMouseCursor() values (optional)
  imguiIO.BackendFlags |=
    ImGuiBackendFlags_PlatformHasViewports;// We can create multi-viewports on the Platform side (optional)*/
}

void ImGuiLayer::OnCreate()
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &imguiIO = ImGui::GetIO();
  imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// NOLINT(hicpp-signed-bitwise) Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  imguiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// NOLINT(hicpp-signed-bitwise) Enable Docking
  imguiIO.ConfigFlags |=// NOLINT(hicpp-signed-bitwise)
    ImGuiConfigFlags_ViewportsEnable;// Enable Multi-Viewport / Platform Windows

  imguiIO.ConfigViewportsNoAutoMerge = true;
  imguiIO.ConfigViewportsNoTaskBarIcon = true;
  imguiIO.ConfigViewportsNoDecoration = false;


  // ImGui::StyleColorsDark();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  // ImGuiStyle &style = ImGui::GetStyle();
  // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
  //  style.WindowRounding = 0.0f;
  //  style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  // }

  InitializeImGuiForJEngine3D();
}
void ImGuiLayer::OnDestroy() {}

void ImGuiLayer::OnUpdate() {}
void ImGuiLayer::OnImGuiRender() {}

void ImGuiLayer::OnEvent(JE::IEvent &event) { JE::UNUSED(event); }


}// namespace JE