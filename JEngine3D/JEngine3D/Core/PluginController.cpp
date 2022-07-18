#include "PluginController.hpp"

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/MemoryController.hpp"

namespace JE {

void NativePluginController::LoadPlugin(const std::filesystem::path &fullPath)
{

  const auto pathString = fullPath.generic_string();

  auto &plugin = m_Plugins.emplace_back();
  auto opened = plugin->PluginHandle.Open(pathString, PLUGIN_FACTORY_FUNCTION);
  if (!opened) {
    Logger::CoreLogger().error("Failed to load plugin: {}", pathString);
    return;
  }

  plugin->PluginInterface = plugin->PluginHandle.CreatePlugin<Scope<INativePlugin, MemoryTag::App>>();
}

// NOLINTNEXTLINE
void NativePluginController::LoadPlugins()
{
  DEBUGBREAK();// TODO(JesusKrists): Implement this
}

void NativePluginController::UpdatePlugins()
{
  if (static_cast<uint64_t>(JE_APP.Uptime()) % PLUGIN_UPDATE_FREQUENCY_MS == 0) {
    ForEach(m_Plugins, [](Scope<PluginEntry, MemoryTag::App> &plugin) { plugin->PluginHandle.Update(); });
  }
}

}// namespace JE