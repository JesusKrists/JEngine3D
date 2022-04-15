#include "ImGuiLayer.hpp"

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Application.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Types.hpp"// for int32_t, basic_st...
#include "JEngine3D/Platform/IPlatformBackend.hpp"// for IPlatformBackend
#include "JEngine3D/Platform/IGraphicsContext.hpp"// for IPlatformBackend
#include "JEngine3D/Core/Events.hpp"// for MousePressEvent
#include "JEngine3D/Core/WindowController.hpp"// for MousePressEvent
#include "JEngine3D/Core/ImGui/ImGuiSupport.hpp"
#include "JEngine3D/Core/ImGui/ImGuiSoftwareRenderer.hpp"// IWYU pragma: keep

#include "Roboto-Regular.embed"

#include <imgui.h>

namespace JE {


/*static void JEngine3DImGuiCreateWindow(ImGuiViewport *viewport)
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
  SDL_CreateWindow("No Title Yet",
    (int)viewport->Pos.x,
    (int)viewport->Pos.y,
    (int)viewport->Size.x,
    (int)viewport->Size.y,
    sdl_flags);

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
}*/

static void InitializeImGuiForJEngine3D()
{
  ImGuiIO &imguiIO = ImGui::GetIO();
  ASSERT(imguiIO.BackendPlatformUserData == nullptr, "Already initialized a platform backend!");

  // Setup backend capabilities flags
  imguiIO.BackendPlatformUserData = static_cast<void *>(&Application::Get());
  imguiIO.BackendPlatformName = "JEngine3D Backend";
  // imguiIO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;// We can honor GetMouseCursor() values (optional)
  // imguiIO.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;// We can honor io.WantSetMousePos requests (optional,
  // rarely used) if (mouse_can_use_global_state)
  //   imguiIO.BackendFlags |=
  //     ImGuiBackendFlags_PlatformHasViewports;// We can create multi-viewports on the Platform side (optional)

  // SDL on Linux/OSX doesn't report events for unfocused windows (see https://github.com/ocornut/imgui/issues/4960)
  // #ifndef __APPLE__
  //   if (mouse_can_use_global_state)
  //     imguiIO.BackendFlags |=
  //       ImGuiBackendFlags_HasMouseHoveredViewport;// We can call io.AddMouseViewportEvent() with correct data
  //       (optional)
  // #endif

  imguiIO.SetClipboardTextFn = [](void *, const char *text) { IPlatformBackend::Get().SetClipboardText(text); };
  imguiIO.GetClipboardTextFn = [](void *) { return IPlatformBackend::Get().ClipboardText(); };

  // Load mouse cursors
  /*bd->MouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  bd->MouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
  bd->MouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
  bd->MouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
  bd->MouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
  bd->MouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
  bd->MouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
  bd->MouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
  bd->MouseCursors[ImGuiMouseCursor_NotAllowed] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);*/

  // Set platform dependent data in viewport
  // Our mouse update function expect PlatformHandle to be filled for the main viewport
  ImGuiViewport *mainViewport = ImGui::GetMainViewport();
  mainViewport->PlatformHandle = const_cast<void *>(// NOLINT(cppcoreguidelines-pro-type-const-cast)
    static_cast<const void *>(&Application::Get().MainWindow()));
  mainViewport->PlatformUserData = nullptr;


  // ImGui_ImplSDL2_UpdateMonitors();

  // Register platform interface (will be coupled with a renderer interface)
  /*ImGuiPlatformIO &platformIO = ImGui::GetPlatformIO();
  platformIO.Platform_CreateWindow = JEngine3DImGuiCreateWindow;
  platformIO.Platform_DestroyWindow = JEngine3DImGuiDestroyWindow;
  platformIO.Platform_ShowWindow = JEngine3DImGuiShowWindow;
  platformIO.Platform_SetWindowPos = JEngine3DImGuiSetWindowPosition;
  platformIO.Platform_GetWindowPos = ImGui_ImplSDL2_GetWindowPos;
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
  // imguiIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;// Enable Multi-Viewport / Platform Windows

  // imguiIO.ConfigViewportsNoAutoMerge = true;
  // imguiIO.ConfigViewportsNoTaskBarIcon = true;
  // imguiIO.ConfigViewportsNoDecoration = false;


  ImGui::StyleColorsDark();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
  // ones. ImGuiStyle &style = ImGui::GetStyle(); if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
  //  style.WindowRounding = 0.0f;
  //  style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  // }

  InitializeImGuiForJEngine3D();

  // Load default font
  ImFontConfig fontConfig;
  fontConfig.FontDataOwnedByAtlas = false;
  fontConfig.GlyphOffset = ImVec2{ 2, 0 };
  ImFont *robotoFont =
    imguiIO.Fonts->AddFontFromMemoryTTF(const_cast<void *>(reinterpret_cast<const void *>(ROBOTO_REGULAR)),// NOLINT
      sizeof(ROBOTO_REGULAR),
      ROBOTO_FONT_PIXEL_SIZE,
      &fontConfig);
  imguiIO.FontDefault = robotoFont;

#if defined(JE_SOFTWARE_CONTEXT)
  ImGuiSoftwareRenderer::Initialize();
#else
  uint8_t *tex_data = nullptr;
  int font_width = 0;
  int font_height = 0;
  imguiIO.Fonts->GetTexDataAsAlpha8(&tex_data, &font_width, &font_height);
#endif
}
void ImGuiLayer::OnDestroy()
{

#if defined(JE_SOFTWARE_CONTEXT)
  ImGuiSoftwareRenderer::Destroy();
#endif

  ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate()
{
  auto &mainWindow = Application::Get().MainWindow();
  auto &graphicsContext = mainWindow.GraphicsContext();

  ImGuiIO &imguiIO = ImGui::GetIO();
  imguiIO.DisplaySize =
    ImVec2{ static_cast<float>(mainWindow.Size().Width), static_cast<float>(mainWindow.Size().Height) };

  imguiIO.DisplayFramebufferScale =
    ImVec2(static_cast<float>(graphicsContext.DrawableSize().Width) / imguiIO.DisplaySize.x,
      static_cast<float>(graphicsContext.DrawableSize().Height) / imguiIO.DisplaySize.y);
  imguiIO.DeltaTime = static_cast<float>(Application::Get().DeltaTime());
}
void ImGuiLayer::OnImGuiRender() {}

void ImGuiLayer::OnEvent(IEvent &event)
{

  ImGuiIO &imguiIO = ImGui::GetIO();

  EventDispatcher dispatcher{ event };

  dispatcher.Dispatch<EventType::WindowResize>([&](const IEvent &evnt) {
    const auto &windowResizeEvent =
      static_cast<const WindowResizeEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    const auto &window = WindowController::Get().WindowFromNativeHandle(windowResizeEvent.NativeWindowHandle());
    if (ImGuiViewport *viewport =
          ImGui::FindViewportByPlatformHandle(const_cast<void *>(// NOLINT(cppcoreguidelines-pro-type-const-cast)
            static_cast<const void *>(&window)))) {
      viewport->PlatformRequestResize = true;
    }

    return false;
  });

  dispatcher.Dispatch<EventType::WindowClose>([&](const IEvent &evnt) {
    const auto &windowCloseEvent =
      static_cast<const WindowCloseEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    const auto &window = WindowController::Get().WindowFromNativeHandle(windowCloseEvent.NativeWindowHandle());
    if (ImGuiViewport *viewport =
          ImGui::FindViewportByPlatformHandle(const_cast<void *>(// NOLINT(cppcoreguidelines-pro-type-const-cast)
            static_cast<const void *>(&window)))) {
      viewport->PlatformRequestClose = true;
    }

    return false;
  });

  dispatcher.Dispatch<EventType::WindowMove>([&](const IEvent &evnt) {
    const auto &windowMoveEvent =
      static_cast<const WindowMoveEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    const auto &window = WindowController::Get().WindowFromNativeHandle(windowMoveEvent.NativeWindowHandle());
    if (ImGuiViewport *viewport =
          ImGui::FindViewportByPlatformHandle(const_cast<void *>(// NOLINT(cppcoreguidelines-pro-type-const-cast)
            static_cast<const void *>(&window)))) {
      viewport->PlatformRequestMove = true;
    }

    return false;
  });

  dispatcher.Dispatch<EventType::WindowFocusGained>([&](const IEvent &evnt) {
    JE::UNUSED(evnt);
    imguiIO.AddFocusEvent(true);
    return false;
  });

  dispatcher.Dispatch<EventType::WindowFocusLost>([&](const IEvent &evnt) {
    JE::UNUSED(evnt);
    imguiIO.AddFocusEvent(false);
    return false;
  });

  dispatcher.Dispatch<EventType::KeyPress>([&](const IEvent &evnt) {
    const auto &keyPressEvent =
      static_cast<const KeyPressEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    imguiIO.AddKeyEvent(ImGuiKey_ModCtrl, keyPressEvent.Modifiers().Ctrl);
    imguiIO.AddKeyEvent(ImGuiKey_ModShift, keyPressEvent.Modifiers().Shift);
    imguiIO.AddKeyEvent(ImGuiKey_ModAlt, keyPressEvent.Modifiers().Alt);
    imguiIO.AddKeyEvent(ImGuiKey_ModSuper, keyPressEvent.Modifiers().Super);
    ImGuiKey key = JEngine3DKeyCodeToImGuiCode(keyPressEvent.Key());
    imguiIO.AddKeyEvent(key, true);

    return imguiIO.WantCaptureKeyboard;// When true we don't capture event in app
  });

  dispatcher.Dispatch<EventType::KeyRelease>([&](const IEvent &evnt) {
    const auto &keyReleaseEvent =
      static_cast<const KeyReleaseEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    imguiIO.AddKeyEvent(ImGuiKey_ModCtrl, keyReleaseEvent.Modifiers().Ctrl);
    imguiIO.AddKeyEvent(ImGuiKey_ModShift, keyReleaseEvent.Modifiers().Shift);
    imguiIO.AddKeyEvent(ImGuiKey_ModAlt, keyReleaseEvent.Modifiers().Alt);
    imguiIO.AddKeyEvent(ImGuiKey_ModSuper, keyReleaseEvent.Modifiers().Super);
    ImGuiKey key = JEngine3DKeyCodeToImGuiCode(keyReleaseEvent.Key());
    imguiIO.AddKeyEvent(key, false);

    return imguiIO.WantCaptureKeyboard;// When true we don't capture event in app
  });

  dispatcher.Dispatch<EventType::TextInput>([&](const IEvent &evnt) {
    const auto &textInputEvent =
      static_cast<const TextInputEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    imguiIO.AddInputCharactersUTF8(std::string(textInputEvent.Text()).c_str());

    return imguiIO.WantCaptureKeyboard;// When true we don't capture event in app
  });

  dispatcher.Dispatch<EventType::MousePress>([&](const IEvent &evnt) {
    const auto &pressEvent =
      static_cast<const MousePressEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    const auto button = MouseButtonToImGuiButton(pressEvent.Button());

    imguiIO.AddMouseButtonEvent(button, true);

    return imguiIO.WantCaptureMouse;// When true we don't capture event in app
  });

  dispatcher.Dispatch<EventType::MouseRelease>([&](const IEvent &evnt) {
    const auto &releaseEvent =
      static_cast<const MouseReleaseEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    const auto button = MouseButtonToImGuiButton(releaseEvent.Button());

    imguiIO.AddMouseButtonEvent(button, false);

    return imguiIO.WantCaptureMouse;// When true we don't capture event in app
  });

  dispatcher.Dispatch<EventType::MouseMove>([&](const IEvent &evnt) {
    const auto &moveEvent =
      static_cast<const MouseMoveEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    imguiIO.AddMousePosEvent(static_cast<float>(moveEvent.Position().X), static_cast<float>(moveEvent.Position().Y));

    return imguiIO.WantCaptureMouse;// When true we don't capture event in app
  });

  dispatcher.Dispatch<EventType::MouseWheel>([&](const IEvent &evnt) {
    const auto &wheelEvent =
      static_cast<const MouseWheelEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    imguiIO.AddMouseWheelEvent(0, static_cast<float>(wheelEvent.ScrollAmount()));

    return imguiIO.WantCaptureMouse;// When true we don't capture event in app
  });
}


void ImGuiLayer::Begin() { ImGui::NewFrame(); }// NOLINT(readability-convert-member-functions-to-static)

void ImGuiLayer::End()// NOLINT(readability-convert-member-functions-to-static)
{
  ImGui::Render();


#if defined(JE_SOFTWARE_CONTEXT)
  ImGuiSoftwareRenderer::RenderImGui(Application::Get().MainWindow().GraphicsContext());
#endif
}


}// namespace JE