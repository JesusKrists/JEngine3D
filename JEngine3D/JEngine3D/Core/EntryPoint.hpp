#pragma once

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/LoggerController.hpp"
#include "JEngine3D/Platform/SDL/SDLPlatformBackend.hpp"
#include "JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContextCreator.hpp"
#include "JEngine3D/Platform/SDL/Software/SDLSoftwareGraphicsContextCreator.hpp"
#include "JEngine3D/Core/WindowController.hpp"
#include "JEngine3D/Core/InputController.hpp"
#include "JEngine3D/Renderer/Software/SoftwareRendererObjectCreator.hpp"

namespace JE {

using Backend = SDLPlatformBackend;

#if defined(JE_SOFTWARE_CONTEXT)
using GraphicsContextCreator = SDLSoftwareGraphicsContextCreator;
using RendererObjectCreator = SoftwareRendererObjectCreator;
#else
using GraphicsContextCreator = SDLGLGraphicsContextCreator;
using RendererAPICreator = OpenGLRendererAPICreator;
#endif

inline auto CreateApplication(const std::string_view &title) -> Scope<Application, MemoryTag::App>
{
  static bool s_EngineInitialized = false;
  ASSERT(!s_EngineInitialized, "Application already created");

  static MemoryController s_MemoryController;
  static LoggerController s_LoggerController;

  static Backend s_PlatformBackend;
  static GraphicsContextCreator s_GraphicsContextCreator;
  static RendererObjectCreator s_RendererObjectCreator;

  static WindowController s_WindowController;
  static InputController s_InputController;

  s_EngineInitialized = true;

  return CreateScope<Application, MemoryTag::App>(title);
}

}// namespace JE