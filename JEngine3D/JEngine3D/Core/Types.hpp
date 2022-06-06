#pragma once

#include <cstdint>// IWYU pragma: export
#include <string>// IWYU pragma: export
#include <string_view>// IWYU pragma: export
#include <fmt/core.h>

#include <glm/glm.hpp>// IWYU pragma: export

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
  constexpr auto operator+=(const Position2DI &other) -> Position2DI &
  {
    this->X += other.X;
    this->Y += other.Y;
    return *this;
  }
};

struct RectangleI
{
  Position2DI Position;// Top left origin
  Size2DI Size;

  constexpr auto operator==(const RectangleI &other) const -> bool = default;
};

struct Range
{
  float Start;
  float End;

  constexpr auto operator==(const Range &other) const -> bool = default;
};

struct Color
{
  constexpr Color(float red, float green, float blue, float alpha) : RawColor(red, green, blue, alpha) {}// NOLINT

  glm::vec4 RawColor;

  [[nodiscard]] constexpr auto ToRGBA8() const -> uint32_t
  {
    return ((static_cast<uint32_t>(RawColor.r * 255 + 0.5F) << 24)// NOLINT
            | (static_cast<uint32_t>(RawColor.g * 255 + 0.5F) << 16)// NOLINT
            | (static_cast<uint32_t>(RawColor.b * 255 + 0.5F) << 8)// NOLINT
            | (static_cast<uint32_t>(RawColor.a * 255 + 0.5F)));// NOLINT
  }
  [[nodiscard]] constexpr auto ToABGR8() const -> uint32_t
  {
    return ((static_cast<uint32_t>(RawColor.a * 255 + 0.5F) << 24)// NOLINT
            | (static_cast<uint32_t>(RawColor.b * 255 + 0.5F) << 16)// NOLINT
            | (static_cast<uint32_t>(RawColor.g * 255 + 0.5F) << 8)// NOLINT
            | (static_cast<uint32_t>(RawColor.r * 255 + 0.5F)));// NOLINT
  }

  constexpr auto operator==(const Color &other) const -> bool = default;
};

struct Vertex
{
  constexpr Vertex(const glm::vec3 &position, const Color &color, const glm::vec2 &uv = { 0, 0 })// NOLINT
    : Position(position), Color(color), UV(uv)
  {}

  glm::vec3 Position;
  JE::Color Color;
  glm::vec2 UV;
};


}// namespace JE

template<> struct fmt::formatter<glm::vec2>
{
  // cppcheck-suppress functionStatic
  template<typename ParseContext> constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

  // cppcheck-suppress functionStatic
  template<typename FormatContext> auto format(const glm::vec2 &vec, FormatContext &ctx)
  {
    return format_to(ctx.out(), "glm::vec2{{ X: {0}, Y: {1} }}", vec.x, vec.y);// NOLINT
  }
};

template<> struct fmt::formatter<glm::vec3>
{
  // cppcheck-suppress functionStatic
  template<typename ParseContext> constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

  // cppcheck-suppress functionStatic
  template<typename FormatContext> auto format(const glm::vec3 &vec, FormatContext &ctx)
  {
    return format_to(ctx.out(), "glm::vec3{{ X: {0}, Y: {1}, Z: {2} }}", vec.x, vec.y, vec.z);// NOLINT
  }
};

template<> struct fmt::formatter<glm::vec4>
{
  // cppcheck-suppress functionStatic
  template<typename ParseContext> constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

  // cppcheck-suppress functionStatic
  template<typename FormatContext> auto format(const glm::vec4 &vec, FormatContext &ctx)
  {
    return format_to(ctx.out(), "glm::vec4{{ X: {0}, Y: {1}, Z: {2}, W: {3} }}", vec.x, vec.y, vec.z, vec.w);// NOLINT
  }
};

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

template<> struct fmt::formatter<JE::Range>
{
  // cppcheck-suppress functionStatic
  template<typename ParseContext> constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

  // cppcheck-suppress functionStatic
  template<typename FormatContext> auto format(const JE::Range &range, FormatContext &ctx)
  {
    return format_to(ctx.out(), "Range{{ Start: {0}, End: {1} }}", range.Start, range.End);
  }
};

template<> struct fmt::formatter<JE::Color>
{
  // cppcheck-suppress functionStatic
  template<typename ParseContext> constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

  // cppcheck-suppress functionStatic
  template<typename FormatContext> auto format(const JE::Color &color, FormatContext &ctx)
  {
    return format_to(ctx.out(),
      "Color{{ R: {0}, G: {1}, B: {2}, A: {3} }}",
      color.RawColor.r,// NOLINT(cppcoreguidelines-pro-type-union-access)
      color.RawColor.g,// NOLINT(cppcoreguidelines-pro-type-union-access)
      color.RawColor.b,// NOLINT(cppcoreguidelines-pro-type-union-access)
      color.RawColor.a);// NOLINT(cppcoreguidelines-pro-type-union-access)
  }
};

template<> struct fmt::formatter<JE::Vertex>
{
  // cppcheck-suppress functionStatic
  template<typename ParseContext> constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

  // cppcheck-suppress functionStatic
  template<typename FormatContext> auto format(const JE::Vertex &vertex, FormatContext &ctx)
  {
    return format_to(
      ctx.out(), "Vertex{{\n{0},\nPosition: {1}\n,UV: {2}\n}}", vertex.Color, vertex.Position, vertex.UV);
  }
};

//////////////////////////////////// Catch2 formatters ///////////////////////////////////////////////

namespace JE {

inline auto operator<<(std::ostream &outStream, glm::vec3 const &value) -> std::ostream &
{
  outStream << fmt::format("{}", value);
  return outStream;
}

inline auto operator<<(std::ostream &outStream, glm::vec4 const &value) -> std::ostream &
{
  outStream << fmt::format("{}", value);
  return outStream;
}

inline auto operator<<(std::ostream &outStream, Size2DI const &value) -> std::ostream &
{
  outStream << fmt::format("{}", value);
  return outStream;
}

inline auto operator<<(std::ostream &outStream, Position2DI const &value) -> std::ostream &
{
  outStream << fmt::format("{}", value);
  return outStream;
}

inline auto operator<<(std::ostream &outStream, RectangleI const &value) -> std::ostream &
{
  outStream << fmt::format("{}", value);
  return outStream;
}

inline auto operator<<(std::ostream &outStream, Range const &value) -> std::ostream &
{
  outStream << fmt::format("{}", value);
  return outStream;
}

inline auto operator<<(std::ostream &outStream, Color const &value) -> std::ostream &
{
  outStream << fmt::format("{}", value);
  return outStream;
}

inline auto operator<<(std::ostream &outStream, Vertex const &value) -> std::ostream &
{
  outStream << fmt::format("{}", value);
  return outStream;
}

}// namespace JE