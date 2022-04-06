#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/WindowController.hpp>

#include "TestPlatformBackend.hpp"// IWYU pragma: export

class TestPlatformBackendFixture
{
public:
  TestPlatformBackendFixture() { JE::UNUSED(m_Backend.Initialize()); }

protected:
  JE::MemoryController m_MemoryController;
  JE::LoggerController m_LoggerController;
  TestPlatformBackend m_Backend;
  JE::WindowController m_WindowController;
  JE::InputController m_InputController;
};