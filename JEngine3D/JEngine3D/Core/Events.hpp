#pragma once

#include "JEngine3D/Platform/IPlatformBackend.hpp"
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/KeyCodes.hpp"// IWYU pragma: export
#include "JEngine3D/Core/MouseButtons.hpp"// IWYU pragma: export

#include <utility>

namespace JE {

enum class EventType {
  Quit,

  WindowResize,
  WindowClose,
  WindowMove,
  WindowHide,
  WindowShow,
  WindowFocusGained,
  WindowFocusLost,

  KeyPress,
  KeyRelease,
  TextInput,

  MousePress,
  MouseRelease,
  MouseMove,
  MouseWheel
};
enum class EventCategory { App, Window, Keyboard, Mouse };

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

  template<EventType type, typename T> inline auto Dispatch(T func) -> bool
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

class IWindowEvent : public IEvent
{
public:
  explicit IWindowEvent(IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

  [[nodiscard]] inline auto Category() const -> EventCategory override { return EventCategory::Window; }
  [[nodiscard]] inline auto NativeWindowHandle() const -> IPlatformBackend::NativeWindowHandle { return m_Handle; }

private:
  IPlatformBackend::NativeWindowHandle m_Handle;
};

class WindowResizeEvent final : public IWindowEvent
{
public:
  WindowResizeEvent(IPlatformBackend::NativeWindowHandle handle, const Size2DI &size)
    : IWindowEvent(handle), m_Size(size)
  {}

  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::WindowResize; }
  [[nodiscard]] inline auto Size() const -> const Size2DI & { return m_Size; }

private:
  Size2DI m_Size;
};

class WindowCloseEvent final : public IWindowEvent
{
  using IWindowEvent::IWindowEvent;

public:
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::WindowClose; }
};

class WindowMoveEvent final : public IWindowEvent
{
public:
  WindowMoveEvent(IPlatformBackend::NativeWindowHandle handle, const Position2DI &position)
    : IWindowEvent(handle), m_Position(position)
  {}

  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::WindowMove; }
  [[nodiscard]] inline auto Position() const -> const Position2DI & { return m_Position; }

private:
  Position2DI m_Position;
};

class WindowHideEvent final : public IWindowEvent
{
  using IWindowEvent::IWindowEvent;

public:
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::WindowHide; }
};

class WindowShowEvent final : public IWindowEvent
{
  using IWindowEvent::IWindowEvent;

public:
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::WindowShow; }
};

class WindowFocusGainedEvent final : public IWindowEvent
{
  using IWindowEvent::IWindowEvent;

public:
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::WindowFocusGained; }
};

class WindowFocusLostEvent final : public IWindowEvent
{
  using IWindowEvent::IWindowEvent;

public:
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::WindowFocusLost; }
};

class IKeyboardEvent : public IEvent
{
public:
  explicit IKeyboardEvent(IPlatformBackend::NativeWindowHandle handle) : m_Handle(handle) {}

  [[nodiscard]] inline auto Category() const -> EventCategory override { return EventCategory::Keyboard; }
  [[nodiscard]] inline auto WindowHandle() const -> IPlatformBackend::NativeWindowHandle { return m_Handle; }

private:
  IPlatformBackend::NativeWindowHandle m_Handle;
};

class KeyPressEvent final : public IKeyboardEvent
{
public:
  KeyPressEvent(IPlatformBackend::NativeWindowHandle handle,
    KeyCode key,
    const KeyModifiers &modifiers = KeyModifiers{},
    int32_t repeat = 0)
    : IKeyboardEvent(handle), m_Key(key), m_Modifiers(modifiers), m_Repeat(repeat)
  {}
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::KeyPress; }

  [[nodiscard]] inline auto Key() const -> KeyCode { return m_Key; }
  [[nodiscard]] inline auto Modifiers() const -> const KeyModifiers & { return m_Modifiers; }
  [[nodiscard]] inline auto Repeat() const -> int32_t { return m_Repeat; }

private:
  KeyCode m_Key;
  KeyModifiers m_Modifiers;
  int32_t m_Repeat;
};

class KeyReleaseEvent final : public IKeyboardEvent
{
public:
  KeyReleaseEvent(IPlatformBackend::NativeWindowHandle handle,
    KeyCode key,
    const KeyModifiers &modifiers = KeyModifiers{},
    int32_t repeat = 0)
    : IKeyboardEvent(handle), m_Key(key), m_Modifiers(modifiers), m_Repeat(repeat)
  {}
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::KeyRelease; }

  [[nodiscard]] inline auto Key() const -> KeyCode { return m_Key; }
  [[nodiscard]] inline auto Modifiers() const -> const KeyModifiers & { return m_Modifiers; }
  [[nodiscard]] inline auto Repeat() const -> int32_t { return m_Repeat; }

private:
  KeyCode m_Key;
  KeyModifiers m_Modifiers;
  int32_t m_Repeat;
};

class TextInputEvent final : public IKeyboardEvent
{
public:
  TextInputEvent(IPlatformBackend::NativeWindowHandle handle, const std::string_view &text)
    : IKeyboardEvent(handle), m_Text(text)
  {}
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::TextInput; }

  [[nodiscard]] inline auto Text() const -> const std::string_view & { return m_Text; }

private:
  std::string_view m_Text;
};

class IMouseEvent : public IEvent
{
public:
  explicit IMouseEvent(IPlatformBackend::NativeWindowHandle handle, const Position2DI &position)
    : m_Handle(handle), m_Position(position)
  {}

  [[nodiscard]] inline auto Category() const -> EventCategory override { return EventCategory::Mouse; }
  [[nodiscard]] inline auto WindowHandle() const -> IPlatformBackend::NativeWindowHandle { return m_Handle; }

  [[nodiscard]] inline auto Position() const -> const Position2DI & { return m_Position; }

private:
  IPlatformBackend::NativeWindowHandle m_Handle;
  Position2DI m_Position;
};

class MousePressEvent final : public IMouseEvent
{
public:
  MousePressEvent(IPlatformBackend::NativeWindowHandle handle,
    const Position2DI &position,
    MouseButton button,
    int32_t clicks)
    : IMouseEvent(handle, position), m_Button(button), m_Clicks(clicks)
  {}
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::MousePress; }

  [[nodiscard]] inline auto Button() const -> MouseButton { return m_Button; }
  [[nodiscard]] inline auto Clicks() const -> int32_t { return m_Clicks; }

private:
  MouseButton m_Button;
  int32_t m_Clicks;
};

class MouseReleaseEvent final : public IMouseEvent
{
public:
  MouseReleaseEvent(IPlatformBackend::NativeWindowHandle handle,
    const Position2DI &position,
    MouseButton button,
    int32_t clicks)
    : IMouseEvent(handle, position), m_Button(button), m_Clicks(clicks)
  {}
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::MouseRelease; }

  [[nodiscard]] inline auto Button() const -> MouseButton { return m_Button; }
  [[nodiscard]] inline auto Clicks() const -> int32_t { return m_Clicks; }

private:
  MouseButton m_Button;
  int32_t m_Clicks;
};

class MouseMoveEvent final : public IMouseEvent
{
public:
  MouseMoveEvent(IPlatformBackend::NativeWindowHandle handle,
    const Position2DI &position,// NOLINT(bugprone-easily-swappable-parameters)
    const Position2DI &relativePosition)
    : IMouseEvent(handle, position), m_RelativePosition(relativePosition)
  {}
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::MouseMove; }

  [[nodiscard]] inline auto RelativePosition() const -> const Position2DI & { return m_RelativePosition; }

private:
  Position2DI m_RelativePosition;
};

class MouseWheelEvent final : public IMouseEvent
{
public:
  explicit MouseWheelEvent(IPlatformBackend::NativeWindowHandle handle,
    int32_t scrollAmount,
    const Position2DI &position = { 0, 0 })
    : IMouseEvent(handle, position), m_ScrollAmount(scrollAmount)
  {}
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::MouseWheel; }

  [[nodiscard]] inline auto ScrollAmount() const -> int32_t { return m_ScrollAmount; }

private:
  int32_t m_ScrollAmount;
};

}// namespace JE