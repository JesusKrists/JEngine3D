#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Types.hpp>

TEST_CASE("JE::Size2DI compares correctly with itself", "[JE::Size2DI]")
{
    static constexpr auto size1 = JE::Size2DI{ 1280, 720 };
    static constexpr auto size2 = JE::Size2DI{ 1280, 720 };

    STATIC_REQUIRE(size1 == size2);
}

TEST_CASE("JE::Position2DI compares correctly with itself", "[JE::Position2DI]")
{
    static constexpr auto position1 = JE::Position2DI{ 1280, 720 };
    static constexpr auto position2 = JE::Position2DI{ 1280, 720 };

    STATIC_REQUIRE(position1 == position2);
}

TEST_CASE("JE::RectangleI compares correctly with itself", "[JE::RectangleI]")
{
    static constexpr auto rect1 = JE::RectangleI{ { 1280, 720 }, { 500, 500 } };
    static constexpr auto rect2 = JE::RectangleI{ { 1280, 720 }, { 500, 500 } };

    STATIC_REQUIRE(rect1 == rect2);
}