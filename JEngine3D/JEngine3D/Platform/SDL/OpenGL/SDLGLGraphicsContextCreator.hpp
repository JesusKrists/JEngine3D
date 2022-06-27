#pragma once

#include "JEngine3D/Platform/IGraphicsContextCreator.hpp"

namespace JE {

class IGraphicsContext;

class SDLGLGraphicsContextCreator final : public IGraphicsContextCreator
{
public:
  [[nodiscard]] auto CreateContext(IPlatformBackend::NativeWindowHandle handle)
    -> Scope<IGraphicsContext, MemoryTag::App> override;
};

}// namespace JE