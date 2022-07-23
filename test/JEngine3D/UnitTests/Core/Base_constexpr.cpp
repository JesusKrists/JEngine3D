#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Base.hpp>

#include <cstddef>// IWYU pragma: keep

TEST_CASE("JE::BIT returns shifted bit from bit index", "[JE::BIT]")
{
    static constexpr auto firstBit  = JE::BIT(0);
    static constexpr auto secondBit = JE::BIT(1);
    static constexpr auto thirdBit  = JE::BIT(2);

    STATIC_REQUIRE(firstBit == 0x01);
    STATIC_REQUIRE(secondBit == 0x02);
    STATIC_REQUIRE(thirdBit == 0x04);
}

TEST_CASE("JE::AlignTo aligns value to a given alignment", "[JE::AlignTo]")
{
    static constexpr size_t val = 0;

    static constexpr size_t val1       = 1;
    static constexpr size_t alignment1 = 4;

    static constexpr size_t val2       = 10;
    static constexpr size_t alignment2 = 8;

    STATIC_REQUIRE(JE::AlignTo(val, alignment1) == 4);

    STATIC_REQUIRE(JE::AlignTo(val1, alignment1) == 4);
    STATIC_REQUIRE(JE::AlignTo(val1, alignment2) == 8);

    STATIC_REQUIRE(JE::AlignTo(val2, alignment1) == 12);
    STATIC_REQUIRE(JE::AlignTo(val2, alignment2) == 16);
}