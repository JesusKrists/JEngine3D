#pragma once

#include <JEngine3D/Platform/IGraphicsContext.hpp>

class TestPlatformGraphicsContext final : public JE::IGraphicsContext
{
    using IGraphicsContext::IGraphicsContext;

public:
    static constexpr auto TEST_DRAWABLE_SIZE = JE::Size2DI{ 640, 480 };

    [[nodiscard]] inline auto DrawableSize() -> JE::Size2DI override { return TEST_DRAWABLE_SIZE; }
    inline void               Resize(const JE::Size2DI& size) override { JE::UNUSED(size); }

    inline void SwapBuffers() override {}
    inline void ResetViewport() override {}

    inline void EnableVSync() override { m_VSyncEnabled = true; }
    inline void DisableVSync() override { m_VSyncEnabled = false; }
    inline auto VSyncEnabled() -> bool override { return m_VSyncEnabled; }

private:
    inline void MakeContextCurrent() override {}

    bool m_VSyncEnabled = false;
};