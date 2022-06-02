#pragma once

#include <string>

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IShader
{
public:
  virtual ~IShader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  [[nodiscard]] virtual auto Name() const -> const std::string & = 0;
};

}// namespace JE