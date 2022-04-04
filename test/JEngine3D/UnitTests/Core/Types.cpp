#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Types.hpp>

TEST_CASE("JE::Size2DI compares correctly with itself", "[JE::Size2DI]")
{
  const auto size1 = JE::Size2DI{ 1280, 720 };
  const auto size2 = JE::Size2DI{ 1280, 720 };

  REQUIRE(size1 == size2);
}

TEST_CASE("JE::Position2DI compares correctly with itself", "[JE::Position2DI]")
{
  const auto position1 = JE::Position2DI{ 1280, 720 };
  const auto position2 = JE::Position2DI{ 1280, 720 };

  REQUIRE(position1 == position2);
}