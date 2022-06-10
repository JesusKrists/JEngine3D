#pragma once

#include "JEngine3D/Platform/IGraphicsContextCreator.hpp"
#include "JEngine3D/Core/MemoryController.hpp"// for Scope, Mem...
#include "JEngine3D/Platform/IPlatformBackend.hpp"// for IPlatformB...

namespace JE {

class IGraphicsContext;

class SDLGLGraphicsContextCreator final : public IGraphicsContextCreator
{
public:
  [[nodiscard]] auto CreateContext(IPlatformBackend::NativeWindowHandle handle)
    -> Scope<IGraphicsContext, MemoryTag::App> override;
};

}// namespace JE