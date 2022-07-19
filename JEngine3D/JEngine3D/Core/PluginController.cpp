#include "PluginController.hpp"

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/MemoryController.hpp"


namespace JE {

void NativePluginController::LoadPlugin(const std::filesystem::path &fullPath)
{
  const auto pathString = fullPath.generic_string();
  m_Plugins.push_back(CreateScope<PluginEntry, MemoryTag::App>());
  auto &plugin = m_Plugins.back();

  auto opened = plugin->PluginHandle.Open(pathString, PLUGIN_FACTORY_FUNCTION_NAME_STR);
  if (!opened) {
    Logger::CoreLogger().error("Failed to load plugin: {}", pathString);
    return;
  }

  plugin->CreatePluginInterface();
  ASSERT(
    MemoryController::Contains(plugin->PluginInterface.get()), "Plugin has to be created with JE::MemoryController");

  plugin->PluginInterface->OnCreate();
}

// NOLINTNEXTLINE
void NativePluginController::LoadPlugins()
{
  DEBUGBREAK();// TODO(JesusKrists): Implement this
}

void NativePluginController::UpdatePlugins()
{
  static auto s_LastUpdateTimestamp = JE_APP.Uptime();
  if (s_LastUpdateTimestamp + PLUGIN_UPDATE_FREQUENCY_S <= JE_APP.Uptime()) {
    ForEach(m_Plugins, [](Scope<PluginEntry, MemoryTag::App> &plugin) {
      if (plugin->PluginHandle.PluginUpdated()) {
        plugin->PluginInterface->PreReload();
        plugin->Reload();
        plugin->PluginInterface->PostReload();
      }
    });
    s_LastUpdateTimestamp = JE_APP.Uptime();
  }
}

}// namespace JE