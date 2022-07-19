#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/Events.hpp"

#include <filesystem>

#include <cr.h>

#define PLUGIN_FACTORY_FUNCTION_NAME CreatePlugin
#define PLUGIN_FACTORY_FUNCTION_NAME_STR "CreatePlugin"// NOLINT
#define PLUGIN_FACTORY_FUNCTION_IMPL(type)                                           \
  extern "C" auto PLUGIN_FACTORY_FUNCTION_NAME()->JE::INativePlugin *                \
  {                                                                                  \
    return new (JE::MemoryController::Allocate<type, JE::MemoryTag::App>(1)) type(); \
  }

#if defined(JE_PLATFORM_WINDOWS)
#ifdef __MINGW32__
#define NATIVE_PLUGIN_NAME(name) "lib" name ".dll"// NOLINT
#else
#define NATIVE_PLUGIN_NAME(name) "" name ".dll"// NOLINT
#endif
#elif defined(JE_PLATFORM_UNIX)
#if defined(JE_PLATFORM_APPLE)
#define NATIVE_PLUGIN_NAME(name) "lib" name ".dylib"// NOLINT
#else
#define NATIVE_PLUGIN_NAME(name) "lib" name ".so"// NOLINT
#endif
#endif

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class INativePlugin
{
public:
  virtual ~INativePlugin() = default;

  virtual void OnCreate() = 0;
  virtual void OnDestroy() = 0;

  virtual void PreReload() = 0;
  virtual void PostReload() = 0;

  virtual void OnUpdate() = 0;
  virtual void OnImGuiRender() = 0;

  virtual void OnEvent(IEvent &event) = 0;
};

class NativePluginController
{
  static constexpr auto PLUGIN_UPDATE_FREQUENCY_S = 0.25;

public:
  // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
  struct PluginEntry
  {
    static constexpr auto RELOAD_TRY_COUNT = 10;

    cr_plugin PluginHandle;
    Scope<INativePlugin, MemoryTag::App> PluginInterface;

    inline void DestroyPluginInterface() { PluginInterface.reset(); }
    inline void CreatePluginInterface() { PluginInterface.reset(PluginHandle.CreatePlugin<INativePlugin *>()); }

    inline void Reload()
    {
      DestroyPluginInterface();
      for (int i = 0; i < RELOAD_TRY_COUNT; i++) {
        if (!PluginHandle.ReloadPlugin()) {
          Logger::CoreLogger().error(
            "Failed to reload plugin (Try: {}/{}): {}", i + 1, RELOAD_TRY_COUNT, PluginHandle.p->fullname);
        } else {
          break;
        }
      }
      CreatePluginInterface();
    }

    ~PluginEntry() { PluginInterface->OnDestroy(); }
  };
  using NativePluginContainer = Vector<Scope<PluginEntry, MemoryTag::App>, MemoryTag::App>;

  void LoadPlugin(const std::filesystem::path &fullPath);
  // cppcheck-suppress functionStatic
  void LoadPlugins();
  void UpdatePlugins();

  [[nodiscard]] inline auto Plugins() -> NativePluginContainer & { return m_Plugins; }

private:
  NativePluginContainer m_Plugins;
};


}// namespace JE