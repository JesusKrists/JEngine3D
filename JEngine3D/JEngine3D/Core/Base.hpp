#pragma once

#ifdef JE_DEBUG
#if defined(JE_PLATFORM_UNIX)
#include <csignal>
#endif
#define JE_ENABLE_ASSERTS
#endif


#define JE_STRINGIFY_MACRO(x) #x

#include <algorithm>


namespace JE {

inline void DEBUGBREAK()
{
#ifdef JE_DEBUG
#if defined(JE_PLATFORM_WINDOWS)
  __debugbreak();
#elif defined(JE_PLATFORM_UNIX)
  raise(SIGTRAP);
#endif
#endif
}

template<typename T> constexpr void UNUSED(T &&val) { (void)val; }

constexpr auto BIT(size_t bitIndex) -> size_t { return static_cast<size_t>(1) << bitIndex; }

constexpr auto AlignTo(size_t value, size_t alignment) -> size_t
{
  if (value == 0) { return alignment; }

  auto remainder = value % alignment;
  if (remainder == 0) { return value; }

  return value + (alignment - remainder);
}

template<typename T, typename Predicate> inline auto FindIf(T &&container, Predicate predicate) -> decltype(auto)
{
  return std::find_if(std::begin(std::forward<T>(container)), std::end(std::forward<T>(container)), predicate);
}

}// namespace JE