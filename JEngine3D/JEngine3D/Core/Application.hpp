#pragma once

#include "JEngine3D/Core/Events.hpp"
#include "JEngine3D/Core/Assert.hpp"// for ASSERT_, ASSERT
#include "JEngine3D/Core/Types.hpp"// for Size2D, int32_t, string_view

namespace JE {

class Window;

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class Application : public IEventProcessor
{
public:
  static constexpr auto DEFAULT_SIZE = Size2DI{ 1280, 720 };

  explicit Application(const std::string_view &title);
  ~Application() override { s_ApplicationInstance = nullptr; }

  [[nodiscard]] static inline auto Get() -> Application &
  {
    ASSERT(s_ApplicationInstance, "Application instance is null");
    return *s_ApplicationInstance;
  }

  void OnEvent(IEvent &event) override;

  [[nodiscard]] inline auto MainWindow() const -> const Window & { return m_MainWindow; }
  [[nodiscard]] inline auto Running() const -> bool { return m_Running; }


  void ProcessMainLoop();
  void Run(int32_t loopCount = -1);

private:
  Window &m_MainWindow;
  bool m_Running = false;

  static Application *s_ApplicationInstance;// NOLINT
};


}// namespace JE