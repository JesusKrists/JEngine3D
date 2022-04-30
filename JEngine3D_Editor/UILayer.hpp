#include <JEngine3D/Core/ILayer.hpp>

namespace JE {
class IEvent;
}

namespace JEditor {

class UILayer final : public JE::ILayer
{
public:
  UILayer() : JE::ILayer("JEditor Test Layer") {}

  void OnCreate() override;
  void OnDestroy() override;

  void OnUpdate() override;
  void OnImGuiRender() override;

  void OnEvent(JE::IEvent &event) override;

private:
  // cppcheck-suppress functionStatic
  void RenderMainMenuBar();
};

}// namespace JEditor