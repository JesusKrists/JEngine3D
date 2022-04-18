#pragma once

#include "JEngine3D/Core/Events.hpp"
#include "JEngine3D/Core/LayerStack.hpp"
#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Types.hpp"// for Size2D, int32_t, string_view
#include "JEngine3D/Core/ImGui/ImGuiLayer.hpp"

namespace JE {


class Window;
class ILayer;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class Application final : public IEventProcessor
{
public:
  static constexpr auto DEFAULT_SIZE = Size2DI{ 640, 480 };

  explicit Application(const std::string_view &title);
  ~Application() override = default;

  [[nodiscard]] static inline auto Get() -> Application &
  {
    ASSERT(s_ApplicationInstance, "Application instance is null");
    return *s_ApplicationInstance;
  }

  void OnEvent(IEvent &event) override;

  void PushLayer(ILayer &layer);
  void PushOverlay(ILayer &layer);
  void PopLayer(ILayer &layer);
  void PopOverlay(ILayer &layer);

  void Run(int32_t loopCount = -1);
  inline void Stop() { m_Running = false; }

  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto MainWindow() -> Window & { return m_MainWindow; }
  [[nodiscard]] inline auto Running() const -> bool { return m_Running; }
  [[nodiscard]] inline auto DeltaTime() const -> double { return m_DeltaTime; }
  [[nodiscard]] inline auto Focused() const -> bool { return m_Focused; }

private:
  void UpdateAppFocus();
  void UpdateDeltaTime();
  void ProcessMainLoop();

  Window &m_MainWindow;
  ImGuiLayer m_ImGuiLayer;
  LayerStack m_LayerStack;

  bool m_Running = false;
  double m_DeltaTime = 0;
  bool m_Focused = false;
  int64_t m_ProcessCount = 0;

  static Application *s_ApplicationInstance;// NOLINT
};


}// namespace JE