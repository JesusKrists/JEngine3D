#include <JEngine3D/Core/ILayer.hpp>

namespace JE {
class IEvent;
}

namespace JEditor {

class TestLayer final : public JE::ILayer
{
public:
  TestLayer() : JE::ILayer("JEditor Test Layer") {}

  void OnCreate() override;
  void OnDestroy() override;

  void OnUpdate() override;
  void OnImGuiRender() override;

  void OnEvent(JE::IEvent &event) override;
};

}// namespace JEditor