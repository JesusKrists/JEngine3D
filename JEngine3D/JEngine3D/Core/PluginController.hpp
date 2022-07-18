#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/Events.hpp"

#include <filesystem>

#include <cr.h>// IWYU pragma: keep

#define PLUGIN_FACTORY_FUNCTION "CreatePlugin"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class INativePlugin
{
public:
  virtual ~INativePlugin() = default;

  virtual void OnCreate() = 0;
  virtual void OnDestroy() = 0;

  virtual void OnUpdate() = 0;
  virtual void OnImGuiRender() = 0;

  virtual void OnEvent(IEvent &event) = 0;
};

class NativePluginController
{
  static constexpr auto PLUGIN_UPDATE_FREQUENCY_MS = 100;

public:
  struct PluginEntry
  {
    cr_plugin PluginHandle;
    Scope<INativePlugin, MemoryTag::App> PluginInterface;
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