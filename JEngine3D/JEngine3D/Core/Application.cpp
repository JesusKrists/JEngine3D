#include "Application.hpp"

#include "JEngine3D/Core/Events.hpp"// for EventDispatcher
#include "JEngine3D/Core/WindowController.hpp"// for WindowController
#include "JEngine3D/Platform/IPlatformBackend.hpp"// for IPlatformBackend

namespace JE {


Application *Application::s_ApplicationInstance = nullptr;// NOLINT

Application::Application(const std::string_view &title)
  : m_MainWindow(WindowController::Get().CreateWindow(title, DEFAULT_SIZE))
{
  ASSERT(!s_ApplicationInstance, "Application instance already exists");
  s_ApplicationInstance = this;
}

void Application::OnEvent(IEvent &event)
{
  if (event.Category() == EventCategory::Window) {
    WindowController::Get().OnEvent(event);
    if (event.Handled()) { return; }
  }

  EventDispatcher dispatcher{ event };

  dispatcher.Dispatch<EventType::Quit>([&](const IEvent &) {
    m_Running = false;
    return true;
  });
}

void Application::ProcessMainLoop() { IPlatformBackend::Get().PollEvents(*this); }

void Application::Run(int32_t loopCount)
{
  ASSERT(loopCount != 0, "Cannot run zero loops");

  m_Running = true;

  if (loopCount < 0) {
    while (m_Running) { ProcessMainLoop(); }
  } else {
    while (m_Running && ((loopCount--) != 0)) { ProcessMainLoop(); }
  }
}

}// namespace JE