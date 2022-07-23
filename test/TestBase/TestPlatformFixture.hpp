#pragma once

#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/WindowController.hpp>
#include <JEngine3D/Core/InputController.hpp>

#include <JEngine3D/Platform/SDL/SDLPlatform.hpp>
#include <JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContextCreator.hpp>
#include <JEngine3D/Renderer/OpenGL/OpenGLRendererObjectCreator.hpp>

#include "TestPlatform.hpp"// IWYU pragma: export
#include "TestPlatformGraphicsContextCreator.hpp"
#include "Renderer/TestRendererObjectCreator.hpp"

#if defined(UNITTEST_BUILD)
using Backend               = TestPlatform;
using BackendContextCreator = TestPlatformGraphicsContextCreator;
using RendererObjectCreator = TestRendererObjectCreator;
#elif defined(INTEGRATIONTEST_BUILD)
using Backend               = JE::SDLPlatform;
using BackendContextCreator = JE::SDLGLGraphicsContextCreator;
using RendererObjectCreator = JE::OpenGLRendererObjectCreator;
#else
#error "Unknown test build"
#endif

class TestPlatformFixture
{
protected:
    JE::MemoryController  m_MemoryController;
    JE::LoggerController  m_LoggerController;
    Backend               m_Backend;
    BackendContextCreator m_ContextCreator;
    RendererObjectCreator m_RendererObjectCreator;
    JE::WindowController  m_WindowController;
    JE::InputController   m_InputController;
};