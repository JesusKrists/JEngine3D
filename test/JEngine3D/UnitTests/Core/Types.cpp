#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Types.hpp>

#include <fmt/core.h>// for format, runtime

TEST_CASE("JE::Size2DI compares correctly with itself", "[JE::Size2DI]")
{
  const auto size1 = JE::Size2DI{ 1280, 720 };
  const auto size2 = JE::Size2DI{ 1280, 720 };

  REQUIRE(size1 == size2);
}

TEST_CASE("JE::Size2DI compares correctly with itself, individual components", "[JE::Size2DI]")
{
  const auto size1 = JE::Size2DI{ 1280, 720 };
  const auto size2 = JE::Size2DI{ 1280, 720 };

  REQUIRE(size1.Width == size2.Width);
  REQUIRE(size1.Height == size2.Height);
}

TEST_CASE("JE::Size2DI is formatted correctly by fmt", "[JE::Size2DI]")
{
  static constexpr auto FORMATTED_STRING = std::string_view{ "Size2DI{ Width: 1280, Height: 720 }" };
  const auto size = JE::Size2DI{ 1280, 720 };
  auto fmtString = fmt::format(fmt::runtime("{}"), size);

  REQUIRE(fmtString == FORMATTED_STRING);
}

TEST_CASE("JE::Position2DI compares correctly with itself", "[JE::Position2DI]")
{
  const auto position1 = JE::Position2DI{ 1280, 720 };
  const auto position2 = JE::Position2DI{ 1280, 720 };

  REQUIRE(position1 == position2);
}

TEST_CASE("JE::Position2DI compares correctly with itself, individual components", "[JE::Position2DI]")
{
  const auto position1 = JE::Position2DI{ 1280, 720 };
  const auto position2 = JE::Position2DI{ 1280, 720 };

  REQUIRE(position1.X == position2.X);
  REQUIRE(position1.Y == position2.Y);
}

TEST_CASE("JE::Position2DI is formatted correctly by fmt", "[JE::Position2DI]")
{
  static constexpr auto FORMATTED_STRING = std::string_view{ "Position2DI{ X: 1280, Y: 720 }" };
  const auto position = JE::Position2DI{ 1280, 720 };
  auto fmtString = fmt::format(fmt::runtime("{}"), position);

  REQUIRE(fmtString == FORMATTED_STRING);
}
