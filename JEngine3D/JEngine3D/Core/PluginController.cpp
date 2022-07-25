#include "PluginController.hpp"

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/MemoryController.hpp"


namespace JE {

    void NativePluginController::LoadPlugin(const std::filesystem::path& fullPath)
    {
        const auto pathString = fullPath.generic_string();
        m_Plugins.push_back(CreateScope<PluginEntry, MemoryTag::App>());
        auto& plugin = m_Plugins.back();

        auto opened = plugin->PluginHandle.Open(pathString, PLUGIN_FACTORY_FUNCTION_NAME_STR);
        if (!opened) {
            Logger::CoreLogger().error("Failed to load plugin: {}", pathString);
            return;
        }

        plugin->CreatePluginInterface();
        ASSERT(MemoryController::Contains(plugin->Implementation.get()), "Plugin has to be created with JE::MemoryController");

        plugin->Implementation->OnCreate(plugin->Interface);
    }

    // NOLINTNEXTLINE
    void NativePluginController::LoadPlugins()
    {
        DEBUGBREAK();// TODO(JesusKrists): Implement this
    }

    void NativePluginController::UpdatePlugins()
    {
        m_PluginRefreshTimer.Execute([&]() {
            ForEach(m_Plugins, [](Scope<PluginEntry, MemoryTag::App>& plugin) {
                if (plugin->PluginHandle.PluginUpdated()) {
                    plugin->Implementation->PreReload(plugin->Interface);
                    plugin->Reload();
                    plugin->Implementation->OnCreate(plugin->Interface);
                }
            });
        });
    }

}// namespace JE