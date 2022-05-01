#pragma once

namespace JE {

enum class MouseButton {

  Left = 1,
  Middle,
  Right,
  X1,
  X2,

  TAG_COUNT_PLUS_ONE
};


inline auto JEngine3DKeyCodeToString(MouseButton button) -> const char *
{
  switch (button) {
  case MouseButton::Left:
    return "Left";
  case MouseButton::Middle:
    return "Middle";
  case MouseButton::Right:
    return "Right";
  case MouseButton::X1:
    return "X1";
  case MouseButton::X2:
    return "X2";
  default:
    return "Unknown";
  }
}

}// namespace JE