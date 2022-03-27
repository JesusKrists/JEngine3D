#pragma once

namespace JE {

enum class EventType { Quit };

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

  //[[nodiscard]] inline auto EventHandled() const -> bool { return m_Handled; }
  // inline void SetEventHandled() { m_Handled = true; }

private:
  // bool m_Handled = false;
};

class QuitEvent final : public IEvent
{
public:
  [[nodiscard]] inline auto Type() const -> EventType override { return EventType::Quit; }
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IEventProcessor
{
public:
  virtual ~IEventProcessor() = default;

  virtual void OnEvent(IEvent &event) = 0;
};

}// namespace JE