#pragma once

#include "JEngine3D/Core/Events.hpp"
#include "JEngine3D/Core/LayerStack.hpp"
#include "JEngine3D/Core/PluginController.hpp"

#include "JEngine3D/Debug/View/ApplicationDebugView.hpp"
#include "JEngine3D/Debug/View/InputControllerDebugView.hpp"
#include "JEngine3D/Debug/View/MemoryControllerDebugView.hpp"
#include "JEngine3D/Debug/View/Renderer2DDebugView.hpp"
#include "JEngine3D/Debug/View/WindowControllerDebugView.hpp"


#include "JEngine3D/Renderer/Renderer2D.hpp"

#include <functional>// for reference_wrapper
#include <filesystem>

namespace JE {
class Application;
}

#define JE_APP JE::Application::Get()// NOLINT

namespace JE {

class Window;
class ILayer;
class IRendererAPI;
class ImGuiLayer;
class IImGuiDebugView;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class Application final : public IEventProcessor
{
  struct InternalDebugViews
  {
    ApplicationDebugView applicationDebugView;
    InputControllerDebugView inputControllerDebugView;
    MemoryControllerDebugView memoryControllerDebugView;
    Renderer2DDebugView renderer2DDebugView;
    WindowControllerDebugView windowControllerDebugView;
  };

  using DebugViewContainer = Vector<std::reference_wrapper<IImGuiDebugView>, MemoryTag::App>;

public:
  static constexpr auto DEFAULT_SIZE = Size2DI{ 640, 480 };
  static constexpr auto MAIN_WINDOW_CONFIG = WindowConfiguration{ true };
  const std::string WORKING_DIRECTORY = std::filesystem::current_path().generic_string();

  explicit Application(const std::string_view &title, bool testMode = false);
  virtual ~Application() = default;// NOLINT

  [[nodiscard]] static inline auto Get() -> Application &
  {
    ASSERT(s_ApplicationInstance, "Application instance is null");
    return *s_ApplicationInstance;
  }

  void OnEvent(IEvent &event) override;

  template<typename DerivedLayer, typename... Args> inline auto PushLayer(Args &&...args) -> DerivedLayer &
  {
    return m_LayerStack.PushLayer<DerivedLayer>(std::forward<Args>(args)...);
  }
  template<typename DerivedLayer, typename... Args> inline auto PushOverlay(Args &&...args) -> DerivedLayer &
  {
    return m_LayerStack.PushOverlay<DerivedLayer>(std::forward<Args>(args)...);
  }

  void PopLayer(ILayer &layer);
  void PopOverlay(ILayer &layer);

  void AddDebugView(IImGuiDebugView &view);

  void Run(int32_t loopCount = -1);
  inline void Stop() { m_Running = false; }

  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto MainWindow() -> Window & { return m_MainWindow; }
  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto RendererAPI() -> JE::IRendererAPI & { return *m_RendererAPI; }
  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto Renderer2D() -> JE::Renderer2D & { return m_Renderer2D; }
  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto ImGuiLayer() -> JE::ImGuiLayer & { return *m_ImGuiLayer; }
  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto Layers() -> LayerStack & { return m_LayerStack; }
  // cppcheck-suppress functionConst
  [[nodiscard]] inline auto DebugViews() -> DebugViewContainer & { return m_DebugViewContainer; }


  [[nodiscard]] inline auto Running() const -> bool { return m_Running; }
  [[nodiscard]] inline auto DeltaTime() const -> double { return m_DeltaTime; }
  [[nodiscard]] inline auto Uptime() const -> double { return m_Uptime; }
  [[nodiscard]] inline auto Focused() const -> bool { return m_Focused; }
  [[nodiscard]] inline auto TotalFrameCount() const -> int64_t { return m_ProcessCount; }

private:
  void AddInternalDebugViews();
  void UpdateAppFocus();
  void UpdateDeltaTime();
  void ProcessMainLoop();

  Window &m_MainWindow;
  Scope<IRendererAPI, MemoryTag::Renderer> m_RendererAPI = IRendererObjectCreator::Get().CreateAPI();
  JE::Renderer2D m_Renderer2D;

  JE::ImGuiLayer *m_ImGuiLayer;
  LayerStack m_LayerStack;

  InternalDebugViews m_InternalDebugViews;
  DebugViewContainer m_DebugViewContainer;

  NativePluginController m_NativePluginController;

  bool m_Running = false;
  double m_DeltaTime = 0;
  double m_Uptime = 0;
  bool m_Focused = false;
  int64_t m_ProcessCount = 0;

  JAPI static Application *s_ApplicationInstance;// NOLINT
};


}// namespace JE