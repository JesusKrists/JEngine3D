#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/WindowController.hpp>
#include <JEngine3D/Core/InputController.hpp>

#include <JEngine3D/Platform/SDL/SDLPlatformBackend.hpp>
#include "TestPlatformBackend.hpp"// IWYU pragma: export

#if defined(UNITTEST_BUILD)
using Backend = TestPlatformBackend;
#elif defined(INTEGRATIONTEST_BUILD)
using Backend = JE::SDLPlatformBackend;
#else
#error "Unknown test build"
#endif

class TestPlatformBackendFixture
{
protected:
  JE::MemoryController m_MemoryController;
  JE::LoggerController m_LoggerController;
  Backend m_Backend;
  JE::WindowController m_WindowController;
  JE::InputController m_InputController;
};