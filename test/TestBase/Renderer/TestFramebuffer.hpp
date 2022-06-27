#pragma once

#include <JEngine3D/Renderer/IFramebuffer.hpp>

class TestFramebuffer final : public JE::IFramebuffer
{
public:
  explicit TestFramebuffer(const JE::FramebufferConfiguration &configuration) : JE::IFramebuffer(configuration) {}

  inline void Bind() override {}
  inline void Unbind() override {}

  inline auto ReadPixel(uint32_t attachmentIndex, const JE::Position2DI &pixel) -> int override
  {
    JE::UNUSED(attachmentIndex);
    JE::UNUSED(pixel);
    return 1;
  }
  inline void ClearAttachment(uint32_t attachmentIndex, int value) override// NOLINT
  {
    JE::UNUSED(attachmentIndex);
    JE::UNUSED(value);
  }
  [[nodiscard]] inline auto AttachmentRendererID(uint32_t attachmentIndex = 0) const -> uint32_t override
  {
    return attachmentIndex;
  }

private:
  inline void ResizeFramebuffer(const JE::Size2DI &size) override { JE::UNUSED(size); }
};