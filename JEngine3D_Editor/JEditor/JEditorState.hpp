#pragma once

#include "main.h"

#include <JEngine3D/Core/Assert.hpp>
#include <JEngine3D/Renderer/IFramebuffer.hpp>
#include <JEngine3D/Renderer/ITexture.hpp>

#include <imgui.h>


namespace JEditor {

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

  JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer> FolderIconTexture;
  JE::Scope<JE::ITexture2D, JE::MemoryTag::Renderer> FileIconTexture;

  //////////////////////////////////////////////////////////

private:
  static EditorState *s_StateInstance;// NOLINT
};

}// namespace JEditor