#pragma once

#include <string>

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IShader
{
public:
  virtual ~IShader() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  [[nodiscard]] virtual auto Name() const -> const std::string & = 0;
};

}// namespace JE