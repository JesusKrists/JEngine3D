#pragma once

#include "JEngine3D/Core/Events.hpp"
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT

#include <unordered_map>

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class InputController final : public IEventProcessor
{
public:
  InputController();
  ~InputController() override;

  [[nodiscard]] static inline auto Get() -> InputController &
  {
    ASSERT(s_InputControllerInstance, "WindowController instance is null");
    return *s_InputControllerInstance;
  }

  void OnEvent(IEvent &event) override;

  void NewFrame();

  [[nodiscard]] auto KeyPressed(KeyCode key) const -> bool;
  [[nodiscard]] auto MousePressed(MouseButton button) const -> bool;

  [[nodiscard]] inline auto MousePosition() const -> const Position2DI & { return m_MousePosition; }
  [[nodiscard]] inline auto RelativeMousePosition() const -> const Position2DI & { return m_RelativeMousePosition; }
  [[nodiscard]] inline auto MouseScrollAmount() const -> int32_t { return m_ScrollAmount; }

  [[nodiscard]] inline auto KeyMap() const -> const std::unordered_map<KeyCode, bool> & { return m_KeyMap; }
  [[nodiscard]] inline auto MouseMap() const -> const std::unordered_map<MouseButton, bool> & { return m_MouseMap; }

private:
  std::unordered_map<KeyCode, bool> m_KeyMap;
  std::unordered_map<MouseButton, bool> m_MouseMap;

  Position2DI m_MousePosition = { 0, 0 };
  Position2DI m_RelativeMousePosition = { 0, 0 };
  int32_t m_ScrollAmount = 0;

  static InputController *s_InputControllerInstance;// NOLINT
};

}// namespace JE