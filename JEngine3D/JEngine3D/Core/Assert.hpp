#pragma once

#include "JEngine3D/Core/LoggerController.hpp"

#include <iostream>// IWYU pragma: export

namespace JE {

template<typename... Args>
inline void ASSERT_([[maybe_unused]] bool check,
  [[maybe_unused]] const char *assertion,// NOLINT(bugprone-easily-swappable-parameters)
  [[maybe_unused]] const char *file,
  [[maybe_unused]] int line,
  [[maybe_unused]] Args &&...args)
{
#if defined(JE_ENABLE_ASSERTS)
  if (!check) {
    JE::Logger::CoreLogger().critical(
      "Assertion '{0}' failed at {1}:{2} | {3}", assertion, file, line, std::forward<Args>(args)...);
    DEBUGBREAK();
  }
#endif
}

}// namespace JE


#define ASSERT(check, ...) JE::ASSERT_(check, JE_STRINGIFY_MACRO(check), __FILE__, __LINE__, __VA_ARGS__)// NOLINT