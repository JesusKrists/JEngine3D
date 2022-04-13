#include "ImGuiLayer.hpp"

#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Application.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Types.hpp"// for int32_t, basic_st...
#include "JEngine3D/Platform/IPlatformBackend.hpp"// for IPlatformBackend
#include "JEngine3D/Core/Events.hpp"// for MousePressEvent

#include <imgui.h>

namespace JE {

static auto JEngine3DKeyCodeToImGuiCode(KeyCode code) -> ImGuiKey
{
  switch (code) {
  case KeyCode::Tab:
    return ImGuiKey_Tab;
  case KeyCode::Left:
    return ImGuiKey_LeftArrow;
  case KeyCode::Right:
    return ImGuiKey_RightArrow;
  case KeyCode::Up:
    return ImGuiKey_UpArrow;
  case KeyCode::Down:
    return ImGuiKey_DownArrow;
  case KeyCode::PageUp:
    return ImGuiKey_PageUp;
  case KeyCode::PageDown:
    return ImGuiKey_PageDown;
  case KeyCode::Home:
    return ImGuiKey_Home;
  case KeyCode::End:
    return ImGuiKey_End;
  case KeyCode::Insert:
    return ImGuiKey_Insert;
  case KeyCode::Delete:
    return ImGuiKey_Delete;
  case KeyCode::Backspace:
    return ImGuiKey_Backspace;
  case KeyCode::Space:
    return ImGuiKey_Space;
  case KeyCode::Return:
    return ImGuiKey_Enter;
  case KeyCode::Escape:
    return ImGuiKey_Escape;
  case KeyCode::Quote:
    return ImGuiKey_Apostrophe;
  case KeyCode::Comma:
    return ImGuiKey_Comma;
  case KeyCode::Minus:
    return ImGuiKey_Minus;
  case KeyCode::Period:
    return ImGuiKey_Period;
  case KeyCode::Slash:
    return ImGuiKey_Slash;
  case KeyCode::Semicolon:
    return ImGuiKey_Semicolon;
  case KeyCode::Equals:
    return ImGuiKey_Equal;
  case KeyCode::Leftbracket:
    return ImGuiKey_LeftBracket;
  case KeyCode::Backslash:
    return ImGuiKey_Backslash;
  case KeyCode::Rightbracket:
    return ImGuiKey_RightBracket;
  case KeyCode::Backquote:
    return ImGuiKey_GraveAccent;
  case KeyCode::Capslock:
    return ImGuiKey_CapsLock;
  case KeyCode::ScrollLock:
    return ImGuiKey_ScrollLock;
  case KeyCode::NumLockClear:
    return ImGuiKey_NumLock;
  case KeyCode::PrintScreen:
    return ImGuiKey_PrintScreen;
  case KeyCode::Pause:
    return ImGuiKey_Pause;
  case KeyCode::KeyPad0:
    return ImGuiKey_Keypad0;
  case KeyCode::KeyPad1:
    return ImGuiKey_Keypad1;
  case KeyCode::KeyPad2:
    return ImGuiKey_Keypad2;
  case KeyCode::KeyPad3:
    return ImGuiKey_Keypad3;
  case KeyCode::KeyPad4:
    return ImGuiKey_Keypad4;
  case KeyCode::KeyPad5:
    return ImGuiKey_Keypad5;
  case KeyCode::KeyPad6:
    return ImGuiKey_Keypad6;
  case KeyCode::KeyPad7:
    return ImGuiKey_Keypad7;
  case KeyCode::KeyPad8:
    return ImGuiKey_Keypad8;
  case KeyCode::KeyPad9:
    return ImGuiKey_Keypad9;
  case KeyCode::KeyPadPeriod:
    return ImGuiKey_KeypadDecimal;
  case KeyCode::KeyPadDivide:
    return ImGuiKey_KeypadDivide;
  case KeyCode::KeyPadMultiply:
    return ImGuiKey_KeypadMultiply;
  case KeyCode::KeyPadMinus:
    return ImGuiKey_KeypadSubtract;
  case KeyCode::KeyPadPlus:
    return ImGuiKey_KeypadAdd;
  case KeyCode::KeyPadEnter:
    return ImGuiKey_KeypadEnter;
  case KeyCode::KeyPadEquals:
    return ImGuiKey_KeypadEqual;
  case KeyCode::LCtrl:
    return ImGuiKey_LeftCtrl;
  case KeyCode::LShift:
    return ImGuiKey_LeftShift;
  case KeyCode::LAlt:
    return ImGuiKey_LeftAlt;
  case KeyCode::LSuper:
    return ImGuiKey_LeftSuper;
  case KeyCode::RCtrl:
    return ImGuiKey_RightCtrl;
  case KeyCode::RShift:
    return ImGuiKey_RightShift;
  case KeyCode::RAlt:
    return ImGuiKey_RightAlt;
  case KeyCode::RSuper:
    return ImGuiKey_RightSuper;
  case KeyCode::Application:
    return ImGuiKey_Menu;
  case KeyCode::Zero:
    return ImGuiKey_0;
  case KeyCode::One:
    return ImGuiKey_1;
  case KeyCode::Two:
    return ImGuiKey_2;
  case KeyCode::Three:
    return ImGuiKey_3;
  case KeyCode::Four:
    return ImGuiKey_4;
  case KeyCode::Five:
    return ImGuiKey_5;
  case KeyCode::Six:
    return ImGuiKey_6;
  case KeyCode::Seven:
    return ImGuiKey_7;
  case KeyCode::Eight:
    return ImGuiKey_8;
  case KeyCode::Nine:
    return ImGuiKey_9;
  case KeyCode::a:
    return ImGuiKey_A;
  case KeyCode::b:
    return ImGuiKey_B;
  case KeyCode::c:
    return ImGuiKey_C;
  case KeyCode::d:
    return ImGuiKey_D;
  case KeyCode::e:
    return ImGuiKey_E;
  case KeyCode::f:
    return ImGuiKey_F;
  case KeyCode::g:
    return ImGuiKey_G;
  case KeyCode::h:
    return ImGuiKey_H;
  case KeyCode::i:
    return ImGuiKey_I;
  case KeyCode::j:
    return ImGuiKey_J;
  case KeyCode::k:
    return ImGuiKey_K;
  case KeyCode::l:
    return ImGuiKey_L;
  case KeyCode::m:
    return ImGuiKey_M;
  case KeyCode::n:
    return ImGuiKey_N;
  case KeyCode::o:
    return ImGuiKey_O;
  case KeyCode::p:
    return ImGuiKey_P;
  case KeyCode::q:
    return ImGuiKey_Q;
  case KeyCode::r:
    return ImGuiKey_R;
  case KeyCode::s:
    return ImGuiKey_S;
  case KeyCode::t:
    return ImGuiKey_T;
  case KeyCode::u:
    return ImGuiKey_U;
  case KeyCode::v:
    return ImGuiKey_V;
  case KeyCode::w:
    return ImGuiKey_W;
  case KeyCode::x:
    return ImGuiKey_X;
  case KeyCode::y:
    return ImGuiKey_Y;
  case KeyCode::z:
    return ImGuiKey_Z;
  case KeyCode::F1:
    return ImGuiKey_F1;
  case KeyCode::F2:
    return ImGuiKey_F2;
  case KeyCode::F3:
    return ImGuiKey_F3;
  case KeyCode::F4:
    return ImGuiKey_F4;
  case KeyCode::F5:
    return ImGuiKey_F5;
  case KeyCode::F6:
    return ImGuiKey_F6;
  case KeyCode::F7:
    return ImGuiKey_F7;
  case KeyCode::F8:
    return ImGuiKey_F8;
  case KeyCode::F9:
    return ImGuiKey_F9;
  case KeyCode::F10:
    return ImGuiKey_F10;
  case KeyCode::F11:
    return ImGuiKey_F11;
  case KeyCode::F12:
    return ImGuiKey_F12;
  default:
    return ImGuiKey_None;
  }
}

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
  imguiIO.BackendPlatformUserData = static_cast<void *>(&JE::Application::Get());
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


  // ImGui::StyleColorsDark();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
  // ones. ImGuiStyle &style = ImGui::GetStyle(); if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
  //  style.WindowRounding = 0.0f;
  //  style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  // }

  InitializeImGuiForJEngine3D();
}
void ImGuiLayer::OnDestroy() {}

void ImGuiLayer::OnUpdate() {}
void ImGuiLayer::OnImGuiRender() {}

void ImGuiLayer::OnEvent(JE::IEvent &event)
{

  ImGuiIO &imguiIO = ImGui::GetIO();

  auto MouseButtonToImGuiButton = [&](MouseButton button) {
    if (button == MouseButton::Left) { return 0; }
    if (button == MouseButton::Right) { return 1; }
    if (button == MouseButton::Middle) { return 2; }
    if (button == MouseButton::X1) { return 3; }
    if (button == MouseButton::X2) { return 4; }

    return -1;
  };


  EventDispatcher dispatcher{ event };
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

  dispatcher.Dispatch<EventType::TextInput>([&](const IEvent &evnt) {
    const auto &textInputEvent =
      static_cast<const TextInputEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    imguiIO.AddInputCharactersUTF8(std::string(textInputEvent.Text()).c_str());

    return imguiIO.WantCaptureKeyboard;// When true we don't capture event in app
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
}


}// namespace JE