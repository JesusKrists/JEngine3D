#pragma once

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/LoggerController.hpp"
#include "JEngine3D/Core/WindowController.hpp"
#include "JEngine3D/Core/InputController.hpp"
#include "JEngine3D/Platform/SDL/SDLPlatformBackend.hpp"
#include "JEngine3D/Platform/SDL/OpenGL/SDLGLGraphicsContextCreator.hpp"
#include "JEngine3D/Renderer/OpenGL/OpenGLRendererObjectCreator.hpp"

namespace JE {

using Backend = SDLPlatformBackend;
using GraphicsContextCreator = SDLGLGraphicsContextCreator;

using RendererObjectCreator = OpenGLRendererObjectCreator;

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