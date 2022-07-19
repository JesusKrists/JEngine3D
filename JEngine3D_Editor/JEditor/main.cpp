#include <JEngine3D/Core/Application.hpp>
#include <stdexcept>

#include "JEditorState.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "UILayer.hpp"

namespace JEditor {

EditorState *EditorState::s_StateInstance = nullptr;// NOLINT
// static UILayer *s_UILayer = nullptr;// NOLINT


class EditorPlugin : public JE::INativePlugin
{
public:
  ~EditorPlugin() { JE::Logger::ClientLogger().debug("~EditorPlugin"); }

  void OnCreate() override
  {
    JE::Logger::ClientLogger().debug(
      "EditorPlugin OnCreate: DLL Application address: {} | Self address: {}", fmt::ptr(&JE_APP), fmt::ptr(this));
  }
  void OnDestroy() override
  {
    JE::Logger::ClientLogger().debug(
      "EditorPlugin OnDestroy: DLL Application address: {} | Self address: {}", fmt::ptr(&JE_APP), fmt::ptr(this));
  }

  void PreReload() override
  {
    JE::Logger::ClientLogger().debug(
      "EditorPlugin PreReload: DLL Application address: {} | Self address: {}", fmt::ptr(&JE_APP), fmt::ptr(this));
  }
  void PostReload() override
  {
    JE::Logger::ClientLogger().debug(
      "EditorPlugin PostReload: DLL Application address: {} | Self address: {}", fmt::ptr(&JE_APP), fmt::ptr(this));
  }

  void OnUpdate() override {}
  void OnImGuiRender() override {}

  void OnEvent(JE::IEvent &event) override { JE::UNUSED(event); }
};

}// namespace JEditor


PLUGIN_FACTORY_FUNCTION_IMPL(JEditor::EditorPlugin)