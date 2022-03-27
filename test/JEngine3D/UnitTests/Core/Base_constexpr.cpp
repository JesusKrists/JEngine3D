#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Base.hpp>

#include <cstddef>// for size_t
#include <initializer_list>// for end

TEST_CASE("JE::BIT returns shifted bit from bit index", "[JE::BIT]")
{
  static constexpr auto firstBit = JE::BIT(0);
  static constexpr auto secondBit = JE::BIT(1);
  static constexpr auto thirdBit = JE::BIT(2);

  STATIC_REQUIRE(firstBit == 0x01);
  STATIC_REQUIRE(secondBit == 0x02);
  STATIC_REQUIRE(thirdBit == 0x04);
}

TEST_CASE("JE::AlignTo aligns value to a given alignment", "[JE::AlignTo]")
{
  static constexpr size_t val = 0;

  static constexpr size_t val1 = 1;
  static constexpr size_t alignment1 = 4;

  static constexpr size_t val2 = 10;
  static constexpr size_t alignment2 = 8;

  STATIC_REQUIRE(JE::AlignTo(val, alignment1) == 4);

  STATIC_REQUIRE(JE::AlignTo(val1, alignment1) == 4);
  STATIC_REQUIRE(JE::AlignTo(val1, alignment2) == 8);

  STATIC_REQUIRE(JE::AlignTo(val2, alignment1) == 12);
  STATIC_REQUIRE(JE::AlignTo(val2, alignment2) == 16);
}
TEST_CASE("JE::FindIf return iterator to correct element", "[JE::FindIf]")
{
  static constexpr auto VALUE_TO_FIND = 5;

  static constexpr auto values = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  static constexpr auto *it = JE::FindIf(values, [](auto value) { return value == VALUE_TO_FIND; });

  STATIC_REQUIRE(it != std::end(values));
  STATIC_REQUIRE(*it == VALUE_TO_FIND);
}