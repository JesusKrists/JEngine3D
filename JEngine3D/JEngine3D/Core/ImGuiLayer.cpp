#include "ImGuiLayer.hpp"

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Application.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/WindowController.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Types.hpp"// for int32_t, basic_st...
#include "JEngine3D/Platform/IPlatformBackend.hpp"// for IPlatformBackend

#include <imgui.h>

namespace JE {

class IEvent;

static void JEngine3DImGuiCreateWindow(ImGuiViewport *viewport)
{
  ImGuiViewport *mainViewport = ImGui::GetMainViewport();
  auto &mainWindow = *static_cast<Window *>(mainViewport->PlatformHandle);
  JE::UNUSED(mainWindow);

  // Share GL resources with main context
  // SDL_GLContext backup_context = NULL;
  // backup_context = SDL_GL_GetCurrentContext();
  // SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
  // SDL_GL_MakeCurrent(main_viewport_data->Window, main_viewport_data->GLContext);

  auto &window = WindowController::Get().CreateWindow("Temporary ImGui Window Title",
    Size2DI{ static_cast<int32_t>(viewport->Size.x), static_cast<int32_t>(viewport->Size.y) });
  /*SDL_CreateWindow("No Title Yet",
    (int)viewport->Pos.x,
    (int)viewport->Pos.y,
    (int)viewport->Size.x,
    (int)viewport->Size.y,
    sdl_flags);*/

  // GLContext = SDL_GL_CreateContext(Window);
  // SDL_GL_SetSwapInterval(0);

  // SDL_GL_MakeCurrent(Window, backup_context);

  viewport->PlatformHandle = static_cast<void *>(&window);
  viewport->PlatformUserData = nullptr;
}

static void JEngine3DImGuiDestroyWindow(ImGuiViewport *viewport)
{
  auto &window = *static_cast<Window *>(viewport->PlatformHandle);
  ASSERT(&window != &JE::Application::Get().MainWindow(), "Cannot destroy MainWindow");

  WindowController::Get().DestroyWindow(window);
  viewport->PlatformUserData = viewport->PlatformHandle = nullptr;
}

static void JEngine3DImGuiShowWindow(ImGuiViewport *viewport)
{
  auto &window = *static_cast<Window *>(viewport->PlatformHandle);
  window.Show();
}

static void JEngine3DImGuiSetWindowPosition(ImGuiViewport *viewport, ImVec2 pos)
{
  auto &window = *static_cast<Window *>(viewport->PlatformHandle);
  window.SetPosition(Position2DI{ static_cast<int32_t>(pos.x), static_cast<int32_t>(pos.y) });
}

static void InitializeImGuiForJEngine3D()
{
  ImGuiIO &imguiIO = ImGui::GetIO();
  ASSERT(imguiIO.BackendPlatformUserData == nullptr, "Already initialized a platform backend!");

  // Setup backend capabilities flags
  // ImGui_ImplSDL2_Data *bd = IM_NEW(ImGui_ImplSDL2_Data)();
  imguiIO.BackendPlatformUserData = static_cast<void *>(&JE::Application::Get());
  imguiIO.BackendPlatformName = "JEngine3D Backend";
  // imguiIO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;// We can honor GetMouseCursor() values (optional)
  //  imguiIO.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;// We can create multi-viewports on the Platform
  //  side (optional)

  imguiIO.SetClipboardTextFn = [](void *, const char *text) { IPlatformBackend::Get().SetClipboardText(text); };
  imguiIO.GetClipboardTextFn = [](void *) { return IPlatformBackend::Get().ClipboardText().data(); };

  // Set platform dependent data in viewport
  // Our mouse update function expect PlatformHandle to be filled for the main viewport
  ImGuiViewport *mainViewport = ImGui::GetMainViewport();
  mainViewport->PlatformHandle = const_cast<void *>(// NOLINT(cppcoreguidelines-pro-type-const-cast)
    static_cast<const void *>(&Application::Get().MainWindow()));
  mainViewport->PlatformUserData = nullptr;

  // Register platform interface (will be coupled with a renderer interface)
  ImGuiPlatformIO &platformIO = ImGui::GetPlatformIO();
  platformIO.Platform_CreateWindow = JEngine3DImGuiCreateWindow;
  platformIO.Platform_DestroyWindow = JEngine3DImGuiDestroyWindow;
  platformIO.Platform_ShowWindow = JEngine3DImGuiShowWindow;
  platformIO.Platform_SetWindowPos = JEngine3DImGuiSetWindowPosition;
  /*platformIO.Platform_GetWindowPos = ImGui_ImplSDL2_GetWindowPos;
  platformIO.Platform_SetWindowSize = ImGui_ImplSDL2_SetWindowSize;
  platformIO.Platform_GetWindowSize = ImGui_ImplSDL2_GetWindowSize;
  platformIO.Platform_SetWindowFocus = ImGui_ImplSDL2_SetWindowFocus;
  platformIO.Platform_GetWindowFocus = ImGui_ImplSDL2_GetWindowFocus;
  platformIO.Platform_GetWindowMinimized = ImGui_ImplSDL2_GetWindowMinimized;
  platformIO.Platform_SetWindowTitle = ImGui_ImplSDL2_SetWindowTitle;
  platformIO.Platform_RenderWindow = ImGui_ImplSDL2_RenderWindow;
  platformIO.Platform_SwapBuffers = ImGui_ImplSDL2_SwapBuffers;*/
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