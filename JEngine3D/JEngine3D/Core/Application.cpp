#include "Application.hpp"

#include "JEngine3D/Core/Events.hpp"// for EventDispatcher
#include "JEngine3D/Core/ILayer.hpp"// for EventDispatcher
#include "JEngine3D/Core/WindowController.hpp"// for WindowController
#include "JEngine3D/Core/InputController.hpp"// for InputController
#include "JEngine3D/Platform/IPlatformBackend.hpp"// for IPlatformBackend
#include "JEngine3D/Platform/IGraphicsContext.hpp"

#include <functional>// for reference_wrapper
#include <vector>// for vector

#include <GL/glew.h>

namespace JE {

Application *Application::s_ApplicationInstance = nullptr;// NOLINT

Application::Application(const std::string_view &title)
  : m_MainWindow(WindowController::Get().CreateWindow(title, DEFAULT_SIZE))
{
  ASSERT(!s_ApplicationInstance, "Application instance already exists");
  s_ApplicationInstance = this;

  PushOverlay(m_ImGuiLayer);

  IPlatformBackend::Get().SetEventProcessor(this);
}

void Application::OnEvent(IEvent &event)
{

  for (const auto &layer : m_LayerStack) { layer.get().OnEvent(event); }

  if (event.Category() == EventCategory::Window) {
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
  UpdateDeltaTime();

  IPlatformBackend::Get().PollEvents();
  if (!m_Running) { return; }

  glClearColor(1.0F, 0.0F, 1.0F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// NOLINT(hicpp-signed-bitwise)

  for (const auto &layer : m_LayerStack) { layer.get().OnUpdate(); }

  for (const auto &layer : m_LayerStack) { layer.get().OnImGuiRender(); }

  m_MainWindow.GraphicsContext().MakeCurrent();
  m_MainWindow.GraphicsContext().SwapBuffers();
}

void Application::Run(int32_t loopCount)
{
  ASSERT(!m_Running, "Engine already running");
  ASSERT(loopCount != 0, "Cannot run zero loops");

  m_Running = true;

  if (loopCount < 0) {
    while (m_Running) { ProcessMainLoop(); }
  } else {
    while (m_Running && ((loopCount--) != 0)) { ProcessMainLoop(); }
  }
}

}// namespace JE