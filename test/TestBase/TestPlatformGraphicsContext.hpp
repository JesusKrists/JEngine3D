#pragma once

#include <JEngine3D/Platform/IGraphicsContext.hpp>

class TestPlatformGraphicsContext : public JE::IGraphicsContext
{
  using IGraphicsContext::IGraphicsContext;

public:
  static constexpr auto TEST_DRAWABLE_SIZE = JE::Size2DI{ 640, 480 };

  [[nodiscard]] inline auto DrawableSize() -> JE::Size2DI override { return TEST_DRAWABLE_SIZE; }
  inline void Resize(const JE::Size2DI &size) override { JE::UNUSED(size); }

  inline void MakeCurrent() override {}
  inline void SwapBuffers() override {}

  inline void Destroy() override {}


  void Clear(const JE::Color &clearColor) override { UNUSED(clearColor); }

  void DrawVerticesIndexed(const JE::Vector<JE::Vertex, JE::MemoryTag::Renderer> &vertices,
    const JE::Vector<uint32_t, JE::MemoryTag::Renderer> &indices) override
  {
    UNUSED(vertices);
    UNUSED(indices);
  }
};