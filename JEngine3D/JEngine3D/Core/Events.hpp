#pragma once

#include "JEngine3D/Platform/IPlatformBackend.hpp"
#include "JEngine3D/Core/Types.hpp"

#include <utility>

namespace JE {

enum class EventType { Quit, WindowResize };
enum class EventCategory { App, Window };

class IEvent
{
public:
  IEvent(const IEvent &other) = delete;
  auto operator=(const IEvent &other) -> IEvent & = delete;
  IEvent(IEvent &&other) = delete;
  auto operator=(IEvent &&other) -> IEvent & = delete;

  IEvent() = default;
  virtual ~IEvent() = default;

  [[nodiscard]] virtual auto Type() const -> EventType = 0;
  [[nodiscard]] virtual auto Category() const -> EventCategory = 0;

  [[nodiscard]] inline auto Handled() const -> bool { return m_Handled; }
  inline void SetEventHandled() { m_Handled = true; }

private:
  bool m_Handled = false;
};

class EventDispatcher
{
public:
  explicit EventDispatcher(IEvent &event) : m_Event(event) { ASSERT(!m_Event.Handled(), "Event already handled"); }

  template<JE::EventType type, typename T> inline auto Dispatch(T func) -> bool
  {
    if (!m_Event.Handled() && m_Event.Type() == type) {
      if (func(std::as_const(m_Event))) {
        m_Event.SetEventHandled();
        return true;
      }
    }
    return false;
  }

private:
  IEvent &m_Event;
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IEventProcessor
{
public:
  virtual ~IEventProcessor() = default;

  virtual void OnEvent(IEvent &event) = 0;
};


class QuitEvent final : public IEvent
{
public:
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::Quit; }
  [[nodiscard]] inline auto Category() const -> EventCategory override { return EventCategory::App; }
};


class WindowResizeEvent final : public IEvent
{
public:
  WindowResizeEvent(IPlatformBackend::NativeWindowHandle handle, const Size2D &size) : m_Handle(handle), m_Size(size) {}

  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::WindowResize; }
  [[nodiscard]] inline auto Category() const -> EventCategory override { return EventCategory::Window; }

  [[nodiscard]] inline auto WindowHandle() const -> IPlatformBackend::NativeWindowHandle { return m_Handle; }
  [[nodiscard]] inline auto Size() const -> const Size2D & { return m_Size; }

private:
  IPlatformBackend::NativeWindowHandle m_Handle;
  Size2D m_Size;
};

}// namespace JE