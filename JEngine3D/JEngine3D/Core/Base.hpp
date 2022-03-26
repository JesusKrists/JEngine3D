#pragma once

#ifdef JE_DEBUG
#if defined(JE_PLATFORM_UNIX)
#include <csignal>
#endif
#define JE_ENABLE_ASSERTS
#endif


#define JE_STRINGIFY_MACRO(x) #x


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

}// namespace JE