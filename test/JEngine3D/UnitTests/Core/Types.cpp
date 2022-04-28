#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Types.hpp>

#include <fmt/core.h>// for format, runtime

TEST_CASE("JE::Size2DI compares correctly with itself", "[JE::Size2DI]")
{
  const auto size1 = JE::Size2DI{ 1280, 720 };
  const auto size2 = JE::Size2DI{ 1280, 720 };

  REQUIRE(size1 == size2);
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

TEST_CASE("JE::Position2DI is formatted correctly by fmt", "[JE::Position2DI]")
{
  static constexpr auto FORMATTED_STRING = std::string_view{ "Position2DI{ X: 1280, Y: 720 }" };
  const auto position = JE::Position2DI{ 1280, 720 };
  auto fmtString = fmt::format(fmt::runtime("{}"), position);

  REQUIRE(fmtString == FORMATTED_STRING);
}

TEST_CASE("JE::RectangleI compares correctly with itself", "[JE::RectangleI]")
{
  const auto rect1 = JE::RectangleI{ { 1280, 720 }, { 500, 500 } };
  const auto rect2 = JE::RectangleI{ { 1280, 720 }, { 500, 500 } };

  REQUIRE(rect1 == rect2);
}

TEST_CASE("JE::RectangleI is formatted correctly by fmt", "[JE::RectangleI]")
{
  static constexpr auto FORMATTED_STRING = std::string_view{ "RectangleI{ X: 1280, Y: 720, X2: 1780, Y2: 1220 }" };
  const auto rect = JE::RectangleI{ { 1280, 720 }, { 500, 500 } };
  auto fmtString = fmt::format(fmt::runtime("{}"), rect);

  REQUIRE(fmtString == FORMATTED_STRING);
}
