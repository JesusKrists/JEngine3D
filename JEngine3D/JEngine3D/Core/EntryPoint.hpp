#pragma once

#include "JEngine3D/Core/Application.hpp"
#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/LoggerController.hpp"
#include "JEngine3D/Platform/SDLPlatformBackend.hpp"
#include "JEngine3D/Core/WindowController.hpp"
#include "JEngine3D/Core/InputController.hpp"

namespace JE {

inline auto CreateApplication(const std::string_view &title) -> Scope<Application, MemoryTag::App>
{
  static bool s_EngineInitialized = false;
  ASSERT(!s_EngineInitialized, "Application already created");

  static MemoryController s_MemoryController;
  static LoggerController s_LoggerController;

  static SDLPlatformBackend s_SDLPlatformBackend;
  JE::UNUSED(s_SDLPlatformBackend.Initialize());

  static WindowController s_WindowController;
  static InputController s_InputController;

  s_EngineInitialized = true;

  return CreateScope<Application, MemoryTag::App>(title);
}

}// namespace JE