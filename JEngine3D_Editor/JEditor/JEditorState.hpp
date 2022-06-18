#pragma once

#include "main.h"

#include <JEngine3D/Core/Assert.hpp>
#include <JEngine3D/Core/MemoryController.hpp>
#include <JEngine3D/Renderer/IFramebuffer.hpp>
#include <JEngine3D/Renderer/ITexture.hpp>

#include <algorithm>
#include <cctype>
#include <imgui.h>
#include <iterator>


namespace JEditor {

enum FileExtension {
  FOLDER,
  SVG,
  JPG,

  UNKNOWN,
  COUNT
};

inline auto StringToFileExtension(std::string extension) -> FileExtension
{
  std::transform(std::begin(extension), std::end(extension), std::begin(extension), ::tolower);

  if (extension == ".svg") { return FileExtension::SVG; }
  if (extension == ".jpg") { return FileExtension::JPG; }

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

  ImFont *DefaultFont12 = nullptr;
  JE::Scope<JE::IFramebuffer, JE::MemoryTag::Renderer> GameViewportFBO;
  JE::UnorderedMap<FileExtension, JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer>, JE::MemoryTag::Editor>
    FileIconMap;

  //////////////////////////////////////////////////////////

private:
  static EditorState *s_StateInstance;// NOLINT
};

}// namespace JEditor