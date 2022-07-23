#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Events.hpp>


TEST_CASE("JE::QuitEvent returns correct event type", "[JE::IEvent]")
{
    JE::QuitEvent event;
    REQUIRE(event.Type() == JE::EventType::Quit);
}


TEST_CASE("JE::EventDispatcher dispatches event correctly based on EventType and marks event as Handled", "[JE::EventDispatcher]")
{
    JE::QuitEvent       event;
    JE::EventDispatcher dispatcher{ event };


    auto dispatched = dispatcher.Dispatch<JE::EventType::Quit>([&](const JE::IEvent&) { return true; });

    REQUIRE(dispatched);
    REQUIRE(event.Handled());


    JE::QuitEvent       event2;
    JE::EventDispatcher dispatcher2{ event2 };

    dispatched = dispatcher2.Dispatch<JE::EventType::WindowResize>([&](const JE::IEvent&) { return true; });

    REQUIRE(!dispatched);
    REQUIRE(!event2.Handled());
}