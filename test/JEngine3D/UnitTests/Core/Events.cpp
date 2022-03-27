#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Events.hpp>

TEST_CASE("JE::QuitEvent returns correct event type", "[JE::IEvent]")
{
  JE::QuitEvent event;
  REQUIRE(event.Type() == JE::EventType::Quit);
}