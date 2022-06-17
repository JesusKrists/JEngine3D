#pragma once

#include "main.h"

#include <JEngine3D/Core/Assert.hpp>

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

  //////////////////////////////////////////////////////////

private:
  static EditorState *s_StateInstance;// NOLINT
};

}// namespace JEditor