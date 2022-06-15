#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/ILayer.hpp>// for ILayer
#include <JEngine3D/Core/LayerStack.hpp>
#include <JEngine3D/Core/MemoryController.hpp>// for Vector
#include <JEngine3D/Core/Types.hpp>// for operator==, char_traits


class LayerStackTestsFixture
{
protected:
  JE::MemoryController m_MemoryController;
};

namespace JE {
class IEvent;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE_METHOD(LayerStackTestsFixture, "JE::LayerStack can push and pop layers and overlays", "[JE::LayerStack]")
{
  static constexpr auto LAYER_NAME = std::string_view{ "Layer Debug Name" };
  static constexpr auto OVERLAY_NAME = std::string_view{ "Overlay Debug Name" };

  class TestLayer : public JE::ILayer
  {
  public:
    explicit TestLayer(const std::string_view &name) : ILayer(name) {}

    void OnCreate() override { m_OnCreateCalled = true; }
    void OnDestroy() override { m_OnDestroyCalled = true; }

    void OnUpdate() override {}
    void OnImGuiRender() override {}

    void OnEvent(JE::IEvent &event) override { JE::UNUSED(event); }

    [[nodiscard]] inline auto OnCreateCalled() const -> bool { return m_OnCreateCalled; }
    [[nodiscard]] inline auto OnDestroyCalled() const -> bool { return m_OnDestroyCalled; }

  private:
    bool m_OnCreateCalled = false;
    bool m_OnDestroyCalled = false;
  };


  JE::LayerStack layerStack;

  auto &testLayer = layerStack.PushLayer<TestLayer>(LAYER_NAME);
  auto &testOverlay = layerStack.PushOverlay<TestLayer>(OVERLAY_NAME);

  REQUIRE(testLayer.DebugName() == LAYER_NAME);
  REQUIRE(testOverlay.DebugName() == OVERLAY_NAME);


  REQUIRE(!layerStack.Layers().empty());
  REQUIRE(testLayer.OnCreateCalled());
  REQUIRE(testOverlay.OnCreateCalled());

  layerStack.PopLayer(testLayer);
  layerStack.PopOverlay(testOverlay);

  REQUIRE(layerStack.Layers().empty());
  REQUIRE(testLayer.OnDestroyCalled());
  REQUIRE(testOverlay.OnDestroyCalled());
}