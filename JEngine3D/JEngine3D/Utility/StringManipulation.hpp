#pragma once

#include <cctype>
#include <iterator>
#include <string>
#include <algorithm>

namespace JE {

inline auto ToUpper(const std::string_view &str) -> std::string
{
  std::string extension;
  std::transform(std::begin(str), std::end(str), std::back_inserter(extension), [](unsigned char c) {
    return static_cast<unsigned char>(std::toupper(c));
  });
  return extension;
}


inline auto ToLower(const std::string_view &str) -> std::string
{
  std::string extension;
  std::transform(std::begin(str), std::end(str), std::back_inserter(extension), [](unsigned char c) {
    return static_cast<unsigned char>(std::tolower(c));
  });
  return extension;
}

}// namespace JE