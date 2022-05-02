#pragma once

#include "JEngine3D/Core/Events.hpp"
#include "JEngine3D/Core/LayerStack.hpp"
#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/Types.hpp"// for Size2D, int32_t, string_view
#include "JEngine3D/Core/ImGui/ImGuiLayer.hpp"

#include "JEngine3D/Debug/View/ApplicationDebugView.hpp"
#include "JEngine3D/Debug/View/InputControllerDebugView.hpp"
#include "JEngine3D/Debug/View/MemoryControllerDebugView.hpp"
#include "JEngine3D/Debug/View/WindowControllerDebugView.hpp"
#include "JEngine3D/Platform/IPlatformBackend.hpp"

#include <functional>// for reference_wrapper


namespace JE {


class Window;
class ILayer;
class IImGuiDebugView;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class Application final : public IEventProcessor
{
  struct InternalDebugViews
  {
    ApplicationDebugView applicationDebugView;
    InputControllerDebugView inputControllerDebugView;
    MemoryControllerDebugView memoryControllerDebugView;
    WindowControllerDebugView windowControllerDebugView;
  };

  using DebugViewContainer = Vector<std::reference_wrapper<IImGuiDebugView>, MemoryTag::App>;

public:
  static constexpr auto DEFAULT_SIZE = Size2DI{ 640, 480 };
  static constexpr auto MAIN_WINDOW_CONFIG = WindowConfiguration{ true };

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

  void AddDebugView(IImGuiDebugView &view);

  void Run(int32_t loopCount = -1);
  inline void Stop() { m_Running = false; }

  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto MainWindow() -> Window & { return m_MainWindow; }
  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto Layers() -> LayerStack & { return m_LayerStack; }
  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto DebugViews() -> DebugViewContainer & { return m_DebugViewContainer; }


  [[nodiscard]] inline auto Running() const -> bool { return m_Running; }
  [[nodiscard]] inline auto DeltaTime() const -> double { return m_DeltaTime; }
  [[nodiscard]] inline auto Focused() const -> bool { return m_Focused; }
  [[nodiscard]] inline auto TotalFrameCount() const -> int64_t { return m_ProcessCount; }

private:
  void AddInternalDebugViews();
  void UpdateAppFocus();
  void UpdateDeltaTime();
  void ProcessMainLoop();

  Window &m_MainWindow;

  ImGuiLayer m_ImGuiLayer;
  LayerStack m_LayerStack;

  InternalDebugViews m_InternalDebugViews;
  DebugViewContainer m_DebugViewContainer;

  bool m_Running = false;
  double m_DeltaTime = 0;
  bool m_Focused = false;
  int64_t m_ProcessCount = 0;

  static Application *s_ApplicationInstance;// NOLINT
};


}// namespace JE