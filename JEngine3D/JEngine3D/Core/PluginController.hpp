#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/Events.hpp"
#include "JEngine3D/Core/PeriodicTimer.hpp"

#include <filesystem>

#include <cr.h>

#define PLUGIN_FACTORY_FUNCTION_NAME CreatePlugin
#define PLUGIN_FACTORY_FUNCTION_NAME_STR "CreatePlugin"// NOLINT
#define PLUGIN_FACTORY_FUNCTION_IMPL(type)                                                                                                 \
    extern "C" auto PLUGIN_FACTORY_FUNCTION_NAME()->JE::INativePlugin*                                                                     \
    {                                                                                                                                      \
        return new (JE::MemoryController::Allocate<type, JE::MemoryTag::App>(1)) type();                                                   \
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

    class PluginInterface;

    // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
    class INativePlugin
    {
    public:
        virtual ~INativePlugin() = default;

        virtual void OnCreate(PluginInterface& plugin)  = 0;
        virtual void OnDestroy(PluginInterface& plugin) = 0;

        virtual void PreReload(PluginInterface& plugin) = 0;

        virtual void OnUpdate()      = 0;
        virtual void OnImGuiRender() = 0;

        virtual void OnEvent(IEvent& event) = 0;
    };


    class PluginInterface
    {
    public:
        template<typename T>
        inline auto CreateState() -> T*
        {
            if (!m_StateData) {
                m_StateData = new (MemoryController::Allocate<T, MemoryTag::App>(1)) T();// NOLINT
            }
            return static_cast<T*>(m_StateData);
        }

        template<typename T>
        inline void DestroyState()
        {
            if (m_StateData) {
                static_cast<T*>(m_StateData)->~T();
                MemoryController::Deallocate<T, MemoryTag::App>(static_cast<T*>(m_StateData));
            }
        }

    private:
        void* m_StateData = nullptr;
    };

    class NativePluginController
    {
        static constexpr auto PLUGIN_UPDATE_FREQUENCY_MS = 250;

    public:
        // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
        struct PluginEntry
        {
            static constexpr auto RELOAD_TRY_COUNT = 10;

            cr_plugin                            PluginHandle;
            Scope<INativePlugin, MemoryTag::App> Implementation;
            PluginInterface                      Interface;

            inline void DestroyPluginInterface() { Implementation.reset(); }
            inline void CreatePluginInterface() { Implementation.reset(PluginHandle.CreatePlugin<INativePlugin*>()); }

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

            ~PluginEntry() { Implementation->OnDestroy(Interface); }
        };
        using NativePluginContainer = Vector<Scope<PluginEntry, MemoryTag::App>, MemoryTag::App>;

        static auto Create() -> Scope<NativePluginController, MemoryTag::App>
        {
            return CreateScope<NativePluginController, MemoryTag::App>();
        }

        void LoadPlugin(const std::filesystem::path& fullPath);
        // cppcheck-suppress functionStatic
        void LoadPlugins();
        void UpdatePlugins();

        // cppcheck-suppress functionConst
        [[nodiscard]] inline auto Plugins() -> NativePluginContainer& { return m_Plugins; }

    private:
        NativePluginContainer m_Plugins;
        PeriodicTimer         m_PluginRefreshTimer{ PLUGIN_UPDATE_FREQUENCY_MS };
    };


}// namespace JE