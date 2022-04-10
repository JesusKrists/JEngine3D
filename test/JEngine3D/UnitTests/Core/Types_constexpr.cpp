#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Types.hpp>

TEST_CASE("JE::Size2DI compares correctly with itself", "[JE::Size2DI]")
{
  static constexpr auto size1 = JE::Size2DI{ 1280, 720 };
  static constexpr auto size2 = JE::Size2DI{ 1280, 720 };

  STATIC_REQUIRE(size1 == size2);
}

TEST_CASE("JE::Size2DI compares correctly with itself, individual components", "[JE::Size2DI]")
{
  static constexpr auto size1 = JE::Size2DI{ 1280, 720 };
  static constexpr auto size2 = JE::Size2DI{ 1280, 720 };

  STATIC_REQUIRE(size1.Width == size2.Width);
  STATIC_REQUIRE(size1.Height == size2.Height);
}

TEST_CASE("JE::Position2DI compares correctly with itself", "[JE::Position2DI]")
{
  static constexpr auto position1 = JE::Position2DI{ 1280, 720 };
  static constexpr auto position2 = JE::Position2DI{ 1280, 720 };

  STATIC_REQUIRE(position1 == position2);
}

TEST_CASE("JE::Position2DI compares correctly with itself, individual components", "[JE::Position2DI]")
{
  static constexpr auto position1 = JE::Position2DI{ 1280, 720 };
  static constexpr auto position2 = JE::Position2DI{ 1280, 720 };

  STATIC_REQUIRE(position1.X == position2.X);
  STATIC_REQUIRE(position1.Y == position2.Y);
}
