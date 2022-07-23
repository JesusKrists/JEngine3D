#pragma once

#include "JEngine3D/Platform/IPlatform.hpp"
#include "JEngine3D/Core/MemoryController.hpp"// for Scope, MemoryTag

namespace JE {

    class IGraphicsContext;

    // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
    class IGraphicsContextCreator
    {
    public:
        using NativeContextHandle = void*;

        IGraphicsContextCreator()
        {
            ASSERT(!s_GraphicsContextCreatorInstance, "Graphics Context Creator instance already created");
            s_GraphicsContextCreatorInstance = this;
        }

        [[nodiscard]] static inline auto Get() -> IGraphicsContextCreator&
        {
            ASSERT(s_GraphicsContextCreatorInstance, "Graphics Context Creator instance is null");
            return *s_GraphicsContextCreatorInstance;
        }

        virtual ~IGraphicsContextCreator() { s_GraphicsContextCreatorInstance = nullptr; };


        [[nodiscard]] virtual auto CreateContext(IPlatform::NativeWindowHandle handle) -> Scope<IGraphicsContext, MemoryTag::App> = 0;

    private:
        JAPI static IGraphicsContextCreator* s_GraphicsContextCreatorInstance;// NOLINT
    };

}// namespace JE