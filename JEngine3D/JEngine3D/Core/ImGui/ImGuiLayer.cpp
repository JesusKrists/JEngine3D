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


#include "Roboto-Regular.embed"

#include <imgui.h>
#include <imgui_internal.h>
#include <array>
#include <cstddef>// IWYU pragma: keep

#include <Tracy.hpp>

namespace JE {

static std::array<bool, static_cast<size_t>(MouseButton::TAG_COUNT_PLUS_ONE) - 1> s_MouseButtonsPressed{};// NOLINT

static auto MouseButtonsPressed() -> bool
{
  for (auto button : s_MouseButtonsPressed) {// NOLINT(readability-use-anyofallof)
    // cppcheck-suppress useStlAlgorithm
    if (button) { return true; }
  }

  return false;
}

static void JEngine3DImGuiCreateWindow(ImGuiViewport *viewport)
{
  WindowConfiguration config{};
  config.Decorated = (viewport->Flags & ImGuiViewportFlags_NoDecoration)// NOLINT(hicpp-signed-bitwise)
                     != ImGuiViewportFlags_NoDecoration;
  auto &window = WindowController::Get().CreateWindow("Temporary ImGui Window Title",
    Size2DI{ static_cast<int32_t>(viewport->Size.x), static_cast<int32_t>(viewport->Size.y) },
    Position2DI{ static_cast<int32_t>(viewport->Pos.x), static_cast<int32_t>(viewport->Pos.y) },
    config);

  viewport->PlatformHandle = static_cast<void *>(&window);
  viewport->PlatformUserData = nullptr;
}

static void JEngine3DImGuiDestroyWindow(ImGuiViewport *viewport)
{
  if (viewport->PlatformUserData != nullptr) {
    viewport->PlatformUserData = viewport->PlatformHandle = nullptr;
    return;// If PlatformUserData is set, then it is the main window, which our Application owns
  }

  auto &window = *static_cast<Window *>(viewport->PlatformHandle);
  ASSERT(&window != &JE_APP.MainWindow(), "Cannot destroy MainWindow");

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
  window.SetPosition(pos);
}

static auto JEngine3DImGuiGetWindowPosition(ImGuiViewport *viewport) -> ImVec2
{
  const auto &window = *static_cast<const Window *>(viewport->PlatformHandle);
  return window.Position();
}

static void JEngine3DImGuiSetWindowSize(ImGuiViewport *viewport, ImVec2 size)
{
  auto &window = *static_cast<Window *>(viewport->PlatformHandle);
  window.SetSize(size);
}

static auto JEngine3DImGuiGetWindowSize(ImGuiViewport *viewport) -> ImVec2
{
  const auto &window = *static_cast<const Window *>(viewport->PlatformHandle);
  return window.Size();
}

static void JEngine3DImGuiSetWindowFocus(ImGuiViewport *viewport)
{
  auto &window = *static_cast<Window *>(viewport->PlatformHandle);
  window.Focus();
}

static auto JEngine3DImGuiGetWindowFocus(ImGuiViewport *viewport) -> bool
{
  const auto &window = *static_cast<const Window *>(viewport->PlatformHandle);
  return window.Focused();
}

static auto JEngine3DImGuiGetWindowMinimized(ImGuiViewport *viewport) -> bool
{
  const auto &window = *static_cast<const Window *>(viewport->PlatformHandle);
  return window.Minimized();
}

static void JEngine3DImGuiSetWindowTitle(ImGuiViewport *viewport, const char *title)
{
  auto &window = *static_cast<Window *>(viewport->PlatformHandle);
  window.SetTitle(title);
}

static void JEngine3DImGuiPlatformRenderWindow(ImGuiViewport *viewport,
  void *)// NOLINT(readability-named-parameter, hicpp-named-parameter)
{
  auto &window = *static_cast<Window *>(viewport->PlatformHandle);
  window.GraphicsContext().MakeCurrent();
}

static void JEngine3DImGuiSwapBuffers(ImGuiViewport *viewport,
  void *)// NOLINT(readability-named-parameter, hicpp-named-parameter)
{
  auto &window = *static_cast<Window *>(viewport->PlatformHandle);
  window.GraphicsContext().MakeCurrent();
  window.GraphicsContext().SwapBuffers();
}

static void JEngine3DImGuiRendererRenderWindow(ImGuiViewport *viewport,
  void *)// NOLINT(readability-named-parameter, hicpp-named-parameter)
{
  JE_APP.ImGuiLayer().Renderer().RenderDrawData(*viewport->DrawData);
}

static void InitializeImGuiForJEngine3D()
{
  ImGuiIO &imguiIO = ImGui::GetIO();
  ASSERT(imguiIO.BackendPlatformUserData == nullptr, "Already initialized a platform backend!");

  // Setup backend capabilities flags
  imguiIO.BackendPlatformUserData = static_cast<void *>(&JE_APP);
  imguiIO.BackendPlatformName = "JEngine3D Backend";
  imguiIO.BackendRendererName = "JEngine3D Renderer";
  // imguiIO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;// We can honor GetMouseCursor() values (optional)
  imguiIO.BackendFlags |=// NOLINT(hicpp-signed-bitwise)
    ImGuiBackendFlags_PlatformHasViewports;// We can create multi-viewports on the Platform side (optional)
  // imguiIO.BackendFlags |=// NOLINT(hicpp-signed-bitwise)
  //   ImGuiBackendFlags_RendererHasVtxOffset;// We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
  imguiIO.BackendFlags |=// NOLINT(hicpp-signed-bitwise)
    ImGuiBackendFlags_RendererHasViewports;// We can create multi-viewports on the Renderer side (optional)

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
  mainViewport->PlatformHandle = static_cast<void *>(&JE_APP.MainWindow());
  mainViewport->PlatformUserData = static_cast<void *>(&JE_APP.MainWindow());


  // Register platform interface (will be coupled with a renderer interface)
  ImGuiPlatformIO &platformIO = ImGui::GetPlatformIO();

  platformIO.Monitors.resize(0);
  int monitorCount = IPlatformBackend::Get().MonitorCount();
  for (int i = 0; i < monitorCount; i++) {
    auto bounds = IPlatformBackend::Get().DisplayBounds(i);
    auto usableBounds = IPlatformBackend::Get().DisplayUsableBounds(i);

    ImGuiPlatformMonitor monitor;
    monitor.MainPos = ImVec2{ static_cast<float>(bounds.Position.X), static_cast<float>(bounds.Position.Y) };
    monitor.MainSize = ImVec2{ static_cast<float>(bounds.Size.Width), static_cast<float>(bounds.Size.Height) };
    monitor.WorkPos =
      ImVec2{ static_cast<float>(usableBounds.Position.X), static_cast<float>(usableBounds.Position.Y) };
    monitor.WorkSize =
      ImVec2{ static_cast<float>(usableBounds.Size.Width), static_cast<float>(usableBounds.Size.Height) };
    monitor.DpiScale = IPlatformBackend::Get().DisplayDPI(i) / 96.0F;// NOLINT
    if (monitor.DpiScale == 0) { monitor.DpiScale = 96.0F; }// NOLINT
    platformIO.Monitors.push_back(monitor);
  }

  platformIO.Platform_CreateWindow = JEngine3DImGuiCreateWindow;
  platformIO.Platform_DestroyWindow = JEngine3DImGuiDestroyWindow;
  platformIO.Platform_ShowWindow = JEngine3DImGuiShowWindow;
  platformIO.Platform_SetWindowPos = JEngine3DImGuiSetWindowPosition;
  platformIO.Platform_GetWindowPos = JEngine3DImGuiGetWindowPosition;
  platformIO.Platform_SetWindowSize = JEngine3DImGuiSetWindowSize;
  platformIO.Platform_GetWindowSize = JEngine3DImGuiGetWindowSize;
  platformIO.Platform_SetWindowFocus = JEngine3DImGuiSetWindowFocus;
  platformIO.Platform_GetWindowFocus = JEngine3DImGuiGetWindowFocus;
  platformIO.Platform_GetWindowMinimized = JEngine3DImGuiGetWindowMinimized;
  platformIO.Platform_SetWindowTitle = JEngine3DImGuiSetWindowTitle;
  platformIO.Platform_RenderWindow = JEngine3DImGuiPlatformRenderWindow;
  platformIO.Platform_SwapBuffers = JEngine3DImGuiSwapBuffers;
  platformIO.Renderer_RenderWindow = JEngine3DImGuiRendererRenderWindow;
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

  // imguiIO.ConfigViewportsNoAutoMerge = true;
  //   imguiIO.ConfigViewportsNoTaskBarIcon = true;
  // imguiIO.ConfigViewportsNoDecoration = true;
  // imguiIO.ConfigViewportsNoDefaultParent = true;


  ImGui::StyleColorsDark();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if ((imguiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)// NOLINT(hicpp-signed-bitwise)
      == ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0F;
    style.Colors[ImGuiCol_WindowBg].w = 1.0F;
  }

  InitializeImGuiForJEngine3D();
}
void ImGuiLayer::OnDestroy() { ImGui::DestroyContext(); }

void ImGuiLayer::OnUpdate()
{
  ZoneScopedN("ImGuiLayer::OnUpdate");// NOLINT
  auto &mainWindow = JE_APP.MainWindow();
  auto &graphicsContext = mainWindow.GraphicsContext();
  auto windowMinimized = mainWindow.Minimized();

  ImGuiIO &imguiIO = ImGui::GetIO();
  if (!windowMinimized) {
    imguiIO.DisplaySize = mainWindow.Size();
    imguiIO.DisplayFramebufferScale = graphicsContext.DrawableSize();
    imguiIO.DisplayFramebufferScale /= imguiIO.DisplaySize;
  } else {
    imguiIO.DisplaySize = ImVec2{ 0, 0 };
  }

  imguiIO.DeltaTime = static_cast<float>(JE_APP.DeltaTime());

  if (MouseButtonsPressed()) {
    IPlatformBackend::Get().CaptureMouse();
  } else {
    IPlatformBackend::Get().ReleaseMouse();
  }
}
void ImGuiLayer::OnImGuiRender()
{
  // ImGui layer itself won't render anything
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void ImGuiLayer::OnEvent(IEvent &event)
{

  ImGuiIO &imguiIO = ImGui::GetIO();

  EventDispatcher dispatcher{ event };

  dispatcher.Dispatch<EventType::WindowResize>([&](const IEvent &evnt) {
    const auto &windowResizeEvent =
      static_cast<const WindowResizeEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto &window = WindowController::Get().WindowFromNativeHandle(windowResizeEvent.NativeWindowHandle());
    if (ImGuiViewport *viewport = ImGui::FindViewportByPlatformHandle(static_cast<void *>(&window))) {
      viewport->PlatformRequestResize = true;
    }

    return false;
  });

  dispatcher.Dispatch<EventType::WindowClose>([&](const IEvent &evnt) {
    const auto &windowCloseEvent =
      static_cast<const WindowCloseEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto &window = WindowController::Get().WindowFromNativeHandle(windowCloseEvent.NativeWindowHandle());
    if (ImGuiViewport *viewport = ImGui::FindViewportByPlatformHandle(static_cast<void *>(&window))) {
      viewport->PlatformRequestClose = true;
    }

    return false;
  });

  dispatcher.Dispatch<EventType::WindowMove>([&](const IEvent &evnt) {
    const auto &windowMoveEvent =
      static_cast<const WindowMoveEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    auto &window = WindowController::Get().WindowFromNativeHandle(windowMoveEvent.NativeWindowHandle());
    if (ImGuiViewport *viewport = ImGui::FindViewportByPlatformHandle(static_cast<void *>(&window))) {
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

    if (m_CaptureEvents) {
      return imguiIO.WantCaptureKeyboard;// When true we don't capture event in app
    } else {
      return false;
    }
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

    if (m_CaptureEvents) {
      return imguiIO.WantCaptureKeyboard;// When true we don't capture event in app
    } else {
      return false;
    }
  });

  dispatcher.Dispatch<EventType::TextInput>([&](const IEvent &evnt) {
    const auto &textInputEvent =
      static_cast<const TextInputEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    imguiIO.AddInputCharactersUTF8(std::string(textInputEvent.Text()).c_str());

    if (m_CaptureEvents) {
      return imguiIO.WantCaptureKeyboard;// When true we don't capture event in app
    } else {
      return false;
    }
  });

  dispatcher.Dispatch<EventType::MousePress>([&](const IEvent &evnt) {
    const auto &pressEvent =
      static_cast<const MousePressEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    const auto button = MouseButtonToImGuiButton(pressEvent.Button());

    imguiIO.AddMouseButtonEvent(button, true);

    s_MouseButtonsPressed[static_cast<size_t>(pressEvent.Button()) - 1] = true;// NOLINT

    if (m_CaptureEvents) {
      return imguiIO.WantCaptureMouse;// When true we don't capture event in app
    } else {
      return false;
    }
  });

  dispatcher.Dispatch<EventType::MouseRelease>([&](const IEvent &evnt) {
    const auto &releaseEvent =
      static_cast<const MouseReleaseEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    const auto button = MouseButtonToImGuiButton(releaseEvent.Button());

    imguiIO.AddMouseButtonEvent(button, false);

    s_MouseButtonsPressed[static_cast<size_t>(releaseEvent.Button()) - 1] = false;// NOLINT

    if (m_CaptureEvents) {
      return imguiIO.WantCaptureMouse;// When true we don't capture event in app
    } else {
      return false;
    }
  });

  dispatcher.Dispatch<EventType::MouseMove>([&](const IEvent &evnt) {
    const auto &moveEvent =
      static_cast<const MouseMoveEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    if (m_CaptureEvents) {
      Position2DI mousePos = moveEvent.Position();
      if (imguiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {// NOLINT
        const auto &window = WindowController::Get().WindowFromNativeHandle(moveEvent.WindowHandle());
        const auto &windowPosition = window.Position();
        mousePos.X += windowPosition.X;
        mousePos.Y += windowPosition.Y;
      }

      imguiIO.AddMousePosEvent(static_cast<float>(mousePos.X), static_cast<float>(mousePos.Y));

      return imguiIO.WantCaptureMouse;// When true we don't capture event in app
    } else {
      return false;
    }
  });

  dispatcher.Dispatch<EventType::MouseWheel>([&](const IEvent &evnt) {
    const auto &wheelEvent =
      static_cast<const MouseWheelEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

    imguiIO.AddMouseWheelEvent(0, static_cast<float>(wheelEvent.ScrollAmount()));

    if (m_CaptureEvents) {
      return imguiIO.WantCaptureMouse;// When true we don't capture event in app
    } else {
      return false;
    }
  });
}

void ImGuiLayer::SetCaptureEvents(bool capture) { m_CaptureEvents = capture; }

void ImGuiLayer::Begin() { ImGui::NewFrame(); }// NOLINT(readability-convert-member-functions-to-static)

void ImGuiLayer::End()// NOLINT(readability-convert-member-functions-to-static)
{
  ImGui::Render();

  if (!JE_APP.MainWindow().Minimized()) { m_ImGuiRenderer.RenderDrawData(*ImGui::GetDrawData()); }

  const ImGuiIO &imguiIO = ImGui::GetIO();
  if ((imguiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)// NOLINT(hicpp-signed-bitwise)
      == ImGuiConfigFlags_ViewportsEnable) {
    auto *currentContext = IGraphicsContext::CurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    currentContext->MakeCurrent();
  }
}

}// namespace JE