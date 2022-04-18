#pragma once

#include <cstdint>// IWYU pragma: export
#include <string>// IWYU pragma: export
#include <string_view>// IWYU pragma: export
#include <fmt/core.h>

namespace JE {

struct Size2DI
{
  int32_t Width;
  int32_t Height;

  constexpr auto operator==(const Size2DI &other) const -> bool = default;
};

struct Position2DI
{
  int32_t X;
  int32_t Y;

  constexpr auto operator==(const Position2DI &other) const -> bool = default;
};

struct RectangleI
{
  Position2DI Position;// Top left origin
  Size2DI Size;

  constexpr auto operator==(const RectangleI &other) const -> bool = default;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

inline auto operator<<(std::ostream &outStream, Size2DI const &value) -> std::ostream &
{
  outStream << fmt::format(fmt::runtime("Size2DI{{ Width: {0}, Height: {1} }}"), value.Width, value.Height);
  return outStream;
}

inline auto operator<<(std::ostream &outStream, Position2DI const &value) -> std::ostream &
{
  outStream << fmt::format(fmt::runtime("Position2DI{{ X: {0}, Y: {1} }}"), value.X, value.Y);
  return outStream;
}

inline auto operator<<(std::ostream &outStream, RectangleI const &value) -> std::ostream &
{
  outStream << fmt::format(fmt::runtime("RectangleI{{ X: {0}, Y: {1}, X2: {2}, Y2: {3} }}"),
    value.Position.X,
    value.Position.Y,
    value.Position.X + value.Size.Width,
    value.Position.Y + value.Size.Height);
  return outStream;
}

}// namespace JE


template<> struct fmt::formatter<JE::Size2DI>
{
  // cppcheck-suppress functionStatic
  template<typename ParseContext> constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

  // cppcheck-suppress functionStatic
  template<typename FormatContext> auto format(const JE::Size2DI &size, FormatContext &ctx)
  {
    return format_to(ctx.out(), "Size2DI{{ Width: {0}, Height: {1} }}", size.Width, size.Height);
  }
};

template<> struct fmt::formatter<JE::Position2DI>
{
  // cppcheck-suppress functionStatic
  template<typename ParseContext> constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

  // cppcheck-suppress functionStatic
  template<typename FormatContext> auto format(const JE::Position2DI &position, FormatContext &ctx)
  {
    return format_to(ctx.out(), "Position2DI{{ X: {0}, Y: {1} }}", position.X, position.Y);
  }
};

template<> struct fmt::formatter<JE::RectangleI>
{
  // cppcheck-suppress functionStatic
  template<typename ParseContext> constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

  // cppcheck-suppress functionStatic
  template<typename FormatContext> auto format(const JE::RectangleI &rect, FormatContext &ctx)
  {
    return format_to(ctx.out(),
      "RectangleI{{ X: {0}, Y: {1}, X2: {2}, Y2: {3} }}",
      rect.Position.X,
      rect.Position.Y,
      rect.Position.X + rect.Size.Width,
      rect.Position.Y + rect.Size.Height);
  }
};
