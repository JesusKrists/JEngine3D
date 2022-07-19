#include "Application.hpp"

#include "JEngine3D/Core/ILayer.hpp"// for EventDispatcher
#include "JEngine3D/Core/WindowController.hpp"// for WindowController
#include "JEngine3D/Core/InputController.hpp"// for InputController
#include "JEngine3D/Platform/IGraphicsContext.hpp"

#include "JEngine3D/Debug/View/IImGuiDebugView.hpp"

#include "JEngine3D/Renderer/IRendererObjectCreator.hpp"
#include "JEngine3D/Core/ImGui/ImGuiLayer.hpp"// for ImGuiLayer
#include "JEngine3D/Renderer/IRendererAPI.hpp"// for IRendererAPI
#include "JEngine3D/Renderer/ImGui/ImGuiRenderer.hpp"// for ImGuiRenderer


#include <exception>// for exception
#include <fmt/format.h>// for ptr
#include <Tracy.hpp>

namespace JE {

JAPI Application *Application::s_ApplicationInstance = nullptr;// NOLINT

Application::Application(const std::string_view &title, bool testMode)
  : m_MainWindow(WindowController::Get().CreateWindow(title,
    DEFAULT_SIZE,
    IPlatformBackend::WINDOW_CENTER_POSITION,
    MAIN_WINDOW_CONFIG))
{
  ZoneScopedN("Application::Application");// NOLINT

  ASSERT(!s_ApplicationInstance, "Application instance already exists");
  s_ApplicationInstance = this;

  Logger::CoreLogger().debug("Application address: {}", fmt::ptr(this));

  m_ImGuiLayer = &PushOverlay<JE::ImGuiLayer>();
  AddInternalDebugViews();

  IPlatformBackend::Get().SetEventProcessor(this);

  // m_NativePluginController.LoadPlugins();
  if (!testMode) {
#if defined(JE_DEBUG)
    m_NativePluginController.LoadPlugin(WORKING_DIRECTORY + "/" NATIVE_PLUGIN_NAME("JEngine3D_Editord"));
#else
    m_NativePluginController.LoadPlugin(WORKING_DIRECTORY + "/" NATIVE_PLUGIN_NAME("JEngine3D_Editor"));
#endif
  }
}

void Application::OnEvent(IEvent &event)
{
  ReverseForEach(m_LayerStack, [&](const Scope<ILayer, MemoryTag::App> &layer) {
    if (event.Handled()) { return; }
    layer->OnEvent(event);
  });

  if (event.Category() == EventCategory::Window) {
    EventDispatcher dispatcher{ event };
    dispatcher.Dispatch<EventType::WindowClose>([&](const IEvent &evnt) {
      const auto &closeEvent =
        static_cast<const WindowCloseEvent &>(evnt);// NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
      if (closeEvent.NativeWindowHandle() == m_MainWindow.NativeHandle()) { m_Running = false; }
      return false;
    });

    WindowController::Get().OnEvent(event);
    if (event.Handled()) { return; }
  }

  if (event.Category() == EventCategory::Keyboard || event.Category() == EventCategory::Mouse) {
    InputController::Get().OnEvent(event);
    if (event.Handled()) { return; }
  }

  EventDispatcher dispatcher{ event };

  dispatcher.Dispatch<EventType::Quit>([&](const IEvent &) {
    m_Running = false;
    return true;
  });

  ForEach(
    m_NativePluginController.Plugins(), [&](const Scope<NativePluginController::PluginEntry, MemoryTag::App> &plugin) {
      if (event.Handled()) { return; }
      plugin->PluginInterface->OnEvent(event);
    });
}

void Application::PopLayer(ILayer &layer)
{
  ZoneScoped;// NOLINT
  m_LayerStack.PopLayer(layer);
}

void Application::PopOverlay(ILayer &layer)
{
  ZoneScoped;// NOLINT
  m_LayerStack.PopOverlay(layer);
}

void Application::AddDebugView(IImGuiDebugView &view) { m_DebugViewContainer.emplace_back(view); }

void Application::AddInternalDebugViews()
{
  AddDebugView(m_InternalDebugViews.applicationDebugView);
  AddDebugView(m_InternalDebugViews.inputControllerDebugView);
  AddDebugView(m_InternalDebugViews.memoryControllerDebugView);
  AddDebugView(m_InternalDebugViews.renderer2DDebugView);
  AddDebugView(m_InternalDebugViews.windowControllerDebugView);
}

void Application::UpdateAppFocus()
{
  auto *focusedNativeWindow = IPlatformBackend::Get().FocusedWindow();
  m_Focused = focusedNativeWindow != nullptr;
}

void Application::UpdateDeltaTime()
{
  static auto s_LastFrameTicks = IPlatformBackend::Get().CurrentTicks();
  auto currentTicks = IPlatformBackend::Get().CurrentTicks();

  m_DeltaTime =
    static_cast<double>(currentTicks - s_LastFrameTicks) / static_cast<double>(IPlatformBackend::Get().TickFrequency());

  s_LastFrameTicks = currentTicks;
}

void Application::ProcessMainLoop()
{
  ++m_ProcessCount;
  m_Uptime += m_DeltaTime;


  {
    ZoneScopedN("Pre-Frame Setup");// NOLINT

    m_NativePluginController.UpdatePlugins();

    UpdateAppFocus();

    UpdateDeltaTime();

    InputController::Get().NewFrame();
    m_Renderer2D.NewFrame();
  }

  {
    ZoneScopedN("Event Poll");// NOLINT

    IPlatformBackend::Get().PollEvents();
    if (!m_Running) { return; }
  }

  {
    ZoneScopedN("Layer & ImGui Processing");// NOLINT

    {
      ZoneScopedN("Make MainWindow Context Current");// NOLINT
      m_MainWindow.GraphicsContext().MakeCurrent();
      m_RendererAPI->Clear();
    }

    {
      ZoneScopedN("OnUpdate");// NOLINT
      ForEach(m_LayerStack, [](const Scope<ILayer, MemoryTag::App> &layer) { layer->OnUpdate(); });
      ForEach(m_NativePluginController.Plugins(),
        [&](const Scope<NativePluginController::PluginEntry, MemoryTag::App> &plugin) {
          plugin->PluginInterface->OnUpdate();
        });
    }
    {
      ZoneScopedN("ImGuiLayer Process and Render");// NOLINT
      m_ImGuiLayer->Begin();
      {
        ZoneScopedN("OnImGuiRender");// NOLINT
        ForEach(m_LayerStack, [](const Scope<ILayer, MemoryTag::App> &layer) { layer->OnImGuiRender(); });
        ForEach(m_NativePluginController.Plugins(),
          [&](const Scope<NativePluginController::PluginEntry, MemoryTag::App> &plugin) {
            plugin->PluginInterface->OnImGuiRender();
          });
      }
      {
        ZoneScopedN("DebugViewRender");// NOLINT
        ForEach(m_DebugViewContainer, [](IImGuiDebugView &view) {
          if (view.IsOpen()) { view.Render(); }
        });
      }
      m_ImGuiLayer->End();
    }
  }

  {
    ZoneScopedN("MainWindow SwapBuffers");// NOLINT
    m_MainWindow.GraphicsContext().SwapBuffers();
  }

  FrameMark;
}

void Application::Run(int32_t loopCount)
{
  ASSERT(!m_Running, "Engine already running");
  ASSERT(loopCount != 0, "Cannot run zero loops");

  JE_APP.ImGuiLayer().Renderer().Initialize();

  m_Running = true;

  m_MainWindow.Focus();

  if (loopCount < 0) {
    while (m_Running) { ProcessMainLoop(); }
  } else {
    while (m_Running && ((loopCount--) != 0)) { ProcessMainLoop(); }
  }

  m_Running = false;
}

}// namespace JE