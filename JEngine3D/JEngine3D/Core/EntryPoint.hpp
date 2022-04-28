#pragma once

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/LoggerController.hpp"
#include "JEngine3D/Platform/SDL/SDLPlatformBackend.hpp"
#include "JEngine3D/Platform/SDL/SDLGLGraphicsContextCreator.hpp"
#include "JEngine3D/Platform/SDL/SDLSoftwareGraphicsContextCreator.hpp"
#include "JEngine3D/Core/WindowController.hpp"
#include "JEngine3D/Core/InputController.hpp"

namespace JE {

using Backend = SDLPlatformBackend;

#if defined(JE_SOFTWARE_CONTEXT)
using GraphicsContextCreator = SDLSoftwareGraphicsContextCreator;
#else
using GraphicsContextCreator = SDLGLGraphicsContextCreator;
#endif

inline auto CreateApplication(const std::string_view &title) -> Scope<Application, MemoryTag::App>
{
  static bool s_EngineInitialized = false;
  ASSERT(!s_EngineInitialized, "Application already created");

  static MemoryController s_MemoryController;
  static LoggerController s_LoggerController;

  static Backend s_PlatformBackend;
  static GraphicsContextCreator s_GraphicsContextCreator;

  static WindowController s_WindowController;
  static InputController s_InputController;

  s_EngineInitialized = true;

  return CreateScope<Application, MemoryTag::App>(title);
}

}// namespace JE