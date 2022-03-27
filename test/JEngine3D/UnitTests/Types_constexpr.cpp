#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Types.hpp>

TEST_CASE("JE::Size2D compares correctly with itself", "[JE::Size2D]")
{
  static constexpr auto size1 = JE::Size2D{ 1280, 720 };
  static constexpr auto size2 = JE::Size2D{ 1280, 720 };

  STATIC_REQUIRE(size1 == size2);
}