#pragma once

#include "JEngine3D/Core/Base.hpp"

#include <iostream>

namespace JE {

template<typename... Args>
inline void ASSERT_([[maybe_unused]] bool check,
  [[maybe_unused]] const char *assertion,// NOLINT(bugprone-easily-swappable-parameters)
  [[maybe_unused]] const char *file,
  [[maybe_unused]] int line,
  [[maybe_unused]] Args &&...args)
{
#ifdef JE_ENABLE_ASSERTS
  if (!check) {
    // AllLogger::critical("Assertion '{0}' failed at {1}:{2} | {3}", assertion, file, line,
    // std::forward<Args>(args)...); // TODO(JesusKrists) - Log asserts properly
    std::cout << "Assertion '" << assertion << "' failed at " << file << ":" << line << "\n";
    DEBUGBREAK();
  }
#endif
}

}// namespace JE


#define ASSERT(check, ...) JE::ASSERT_(check, JE_STRINGIFY_MACRO(check), __FILE__, __LINE__, __VA_ARGS__)// NOLINT