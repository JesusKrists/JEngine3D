

#include "JEditorState.hpp"
#include "UILayer.hpp"
#include <JEngine3D/Core/Application.hpp>

namespace JEditor {

EditorState *EditorState::s_StateInstance = nullptr;// NOLINT
static UILayer *s_UILayer = nullptr;// NOLINT


// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class EditorPlugin : public JE::INativePlugin
{
public:
  void OnCreate(JE::PluginInterface &plugin) override
  {
    JE::Logger::ClientLogger().debug(
      "EditorPlugin OnCreate: DLL Application address: {} | Self address: {}", fmt::ptr(&JE_APP), fmt::ptr(this));

    EditorState::s_StateInstance = plugin.CreateState<EditorState>();// NOLINT(clang-analyzer-core.CallAndMessage)
    s_UILayer = &JE_APP.PushLayer<UILayer>();
  }
  void OnDestroy(JE::PluginInterface &plugin) override
  {
    JE::Logger::ClientLogger().debug(
      "EditorPlugin OnDestroy: DLL Application address: {} | Self address: {}", fmt::ptr(&JE_APP), fmt::ptr(this));

    plugin.DestroyState<EditorState>();// NOLINT(clang-analyzer-core.CallAndMessage)
    EditorState::s_StateInstance = nullptr;
  }

  void PreReload(JE::PluginInterface &plugin) override
  {
    JE::UNUSED(plugin);

    JE::Logger::ClientLogger().debug(
      "EditorPlugin PreReload: DLL Application address: {} | Self address: {}", fmt::ptr(&JE_APP), fmt::ptr(this));

    JE_APP.PopLayer(*s_UILayer);
    s_UILayer = nullptr;
  }

  void OnUpdate() override {}
  void OnImGuiRender() override {}

  void OnEvent(JE::IEvent &event) override { JE::UNUSED(event); }
};

}// namespace JEditor


PLUGIN_FACTORY_FUNCTION_IMPL(JEditor::EditorPlugin)