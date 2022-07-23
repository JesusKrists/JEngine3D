#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/ILayer.hpp>
#include <JEngine3D/Core/Types.hpp>// for operator==, char_traits, string

namespace JE {
    class IEvent;
}

TEST_CASE("JE::ILayer has debug name", "[JE::ILayer]")
{
    static constexpr auto NAME = std::string_view{ "Layer Debug Name" };

    class TestLayer : public JE::ILayer
    {
    public:
        TestLayer() : ILayer(NAME) {}

        void OnCreate() override {}
        void OnDestroy() override {}

        void OnUpdate() override {}
        void OnImGuiRender() override {}

        void OnEvent(JE::IEvent& event) override { JE::UNUSED(event); }
    } testLayer;

    REQUIRE(testLayer.DebugName() == NAME);
}