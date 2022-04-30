#include "Application.hpp"

#include "JEngine3D/Core/Events.hpp"// for EventDispatcher
#include "JEngine3D/Core/ILayer.hpp"// for EventDispatcher
#include "JEngine3D/Core/WindowController.hpp"// for WindowController
#include "JEngine3D/Core/InputController.hpp"// for InputController
#include "JEngine3D/Platform/IPlatformBackend.hpp"// for IPlatformBackend
#include "JEngine3D/Platform/IGraphicsContext.hpp"

#include "JEngine3D/Debug/View/IImGuiDebugView.hpp"

#include <iterator>// for rbegin, rend

namespace JE {

Application *Application::s_ApplicationInstance = nullptr;// NOLINT

Application::Application(const std::string_view &title)
  : m_MainWindow(WindowController::Get().CreateWindow(title,
    DEFAULT_SIZE,
    IPlatformBackend::WINDOW_CENTER_POSITION,
    MAIN_WINDOW_CONFIG))
{
  ASSERT(!s_ApplicationInstance, "Application instance already exists");
  s_ApplicationInstance = this;

  PushOverlay(m_ImGuiLayer);

  AddDebugView(m_InternalDebugViews.windowControllerDebugView);

  IPlatformBackend::Get().SetEventProcessor(this);
}

void Application::OnEvent(IEvent &event)
{

  for (auto layerIt = std::rbegin(m_LayerStack); layerIt != std::rend(m_LayerStack); ++layerIt) {
    layerIt->get().OnEvent(event);
    if (event.Handled()) { return; }
  }

  if (event.Category() == EventCategory::Window) {
    EventDispatcher dispatcher{ event };
    dispatcher.Dispatch<EventType::WindowClose>([&](const IEvent &evnt) {
      const auto &closeEvent = static_cast<const WindowCloseEvent &>(evnt);// NOLINT(cppcoreguidelines-pro
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
}

void Application::PushLayer(ILayer &layer) { m_LayerStack.PushLayer(layer); }

void Application::PushOverlay(ILayer &layer) { m_LayerStack.PushOverlay(layer); }

void Application::PopLayer(ILayer &layer) { m_LayerStack.PopLayer(layer); }

void Application::PopOverlay(ILayer &layer) { m_LayerStack.PopOverlay(layer); }

void Application::AddDebugView(IImGuiDebugView &view) { m_DebugViewContainer.push_back(view); }

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

  UpdateAppFocus();

  UpdateDeltaTime();

  IPlatformBackend::Get().PollEvents();
  if (!m_Running) { return; }

  ForEach(m_LayerStack, [&](ILayer &layer) { layer.OnUpdate(); });

  m_ImGuiLayer.Begin();
  ForEach(m_LayerStack, [&](ILayer &layer) { layer.OnImGuiRender(); });
  ForEach(m_DebugViewContainer, [&](IImGuiDebugView &view) {
    if (view.IsOpen()) { view.OnImGuiRender(); }
  });
  m_ImGuiLayer.End();

  m_MainWindow.GraphicsContext().MakeCurrent();
  m_MainWindow.GraphicsContext().SwapBuffers();
}

void Application::Run(int32_t loopCount)
{
  ASSERT(!m_Running, "Engine already running");
  ASSERT(loopCount != 0, "Cannot run zero loops");

  m_Running = true;

  m_MainWindow.Focus();

  if (loopCount < 0) {
    while (m_Running) { ProcessMainLoop(); }
  } else {
    while (m_Running && ((loopCount--) != 0)) { ProcessMainLoop(); }
  }
}

}// namespace JE