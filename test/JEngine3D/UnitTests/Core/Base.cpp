#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Base.hpp>

#include <initializer_list>// for end, initializer_list

TEST_CASE("JE::BIT returns shifted bit from bit index", "[JE::BIT]")
{
  const auto firstBit = JE::BIT(0);
  const auto secondBit = JE::BIT(1);
  const auto thirdBit = JE::BIT(2);

  REQUIRE(firstBit == 0x01);
  REQUIRE(secondBit == 0x02);
  REQUIRE(thirdBit == 0x04);
}

TEST_CASE("JE::AlignTo aligns value to a given alignment", "[JE::AlignTo]")
{
  const auto values = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const auto alignments = { 2, 4, 6, 8, 10, 12, 14, 16 };

  for (const auto value : values) {
    for (const auto alignment : alignments) {
      REQUIRE(
        JE::AlignTo(static_cast<size_t>(value), static_cast<size_t>(alignment)) % static_cast<size_t>(alignment) == 0);
    }
  }
}

TEST_CASE("JE::FindIf return iterator to correct element", "[JE::FindIf]")
{
  static constexpr auto VALUE_TO_FIND = 5;

  const auto values = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  const auto *iter = JE::FindIf(values, [](auto value) { return value == VALUE_TO_FIND; });

  REQUIRE(iter != std::end(values));
  REQUIRE(*iter == VALUE_TO_FIND);
}