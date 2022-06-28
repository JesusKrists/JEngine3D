#pragma once

#include "main.h"

#include <JEngine3D/Core/Assert.hpp>
#include <JEngine3D/Renderer/IFramebuffer.hpp>
#include <JEngine3D/Renderer/ITexture.hpp>
#include <JEngine3D/Utility/StringManipulation.hpp>

#include <algorithm>
#include <cctype>
#include <imgui.h>
#include <iterator>


namespace JEditor {

enum FileExtension {
  FOLDER,
  FOLDER_OPEN,
  SVG,
  JPG,
  TTF,
  INI,

  UNKNOWN,
  COUNT
};

inline auto StringToFileExtension(const std::string_view &extension) -> FileExtension
{
  auto extensionLower = JE::ToLower(extension);

  if (extensionLower == ".svg") { return FileExtension::SVG; }
  if (extensionLower == ".jpg") { return FileExtension::JPG; }
  if (extensionLower == ".ttf") { return FileExtension::TTF; }
  if (extensionLower == ".ini") { return FileExtension::INI; }

  return FileExtension::UNKNOWN;
}

struct EditorState
{
  friend auto ::cr_main(cr_plugin *, cr_op operation) -> int;

  [[nodiscard]] static inline auto Get() -> EditorState &
  {
    ASSERT(s_StateInstance, "State instance is null");
    return *s_StateInstance;
  }

  /////////////////////////////////////////////////////////

  ImFont *DefaultFont = nullptr;
  ImFont *DefaultFont12 = nullptr;
  ImFont *Segoe16Bold = nullptr;
  ImFont *Segoe24Bold = nullptr;
  JE::Scope<JE::IFramebuffer, JE::MemoryTag::Renderer> GameViewportFBO;
  JE::UnorderedMap<FileExtension, JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer>, JE::MemoryTag::Editor>
    FileIconMap;

  //////////////////////////////////////////////////////////

private:
  static EditorState *s_StateInstance;// NOLINT
};

}// namespace JEditor