#pragma once

#include <iterator>
#include <string>

namespace JE {

inline auto ToUpper(const std::string_view &str) -> std::string
{
  std::string extension;
  std::transform(std::begin(str), std::end(str), std::back_inserter(extension), ::toupper);
  return extension;
}


inline auto ToLower(const std::string_view &str) -> std::string
{
  std::string extension;
  std::transform(std::begin(str), std::end(str), std::back_inserter(extension), ::tolower);
  return extension;
}

}// namespace JE