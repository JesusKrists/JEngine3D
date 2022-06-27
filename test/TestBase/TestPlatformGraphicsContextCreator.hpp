#pragma once

#include <JEngine3D/Platform/IGraphicsContextCreator.hpp>
#include "TestPlatformGraphicsContext.hpp"

class TestPlatformGraphicsContextCreator final : public JE::IGraphicsContextCreator
{
public:
  [[nodiscard]] inline auto CreateContext(JE::IPlatformBackend::NativeWindowHandle handle)
    -> JE::Scope<JE::IGraphicsContext, JE::MemoryTag::App> override
  {
    return JE::CreatePolymorphicScope<TestPlatformGraphicsContext, JE::MemoryTag::App, JE::IGraphicsContext>(
      handle, reinterpret_cast<JE::IGraphicsContextCreator::NativeContextHandle>(++m_CurrentContextID));// NOLINT
  }

private:
  size_t m_CurrentContextID = 0;
};