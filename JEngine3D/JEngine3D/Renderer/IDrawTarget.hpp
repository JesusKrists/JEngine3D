#pragma once

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IDrawTarget
{
public:
  virtual ~IDrawTarget() = default;

  virtual void Bind() = 0;
  virtual void UnBind() = 0;
};

}// namespace JE