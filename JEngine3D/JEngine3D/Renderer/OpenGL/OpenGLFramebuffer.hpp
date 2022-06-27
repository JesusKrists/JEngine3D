#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Renderer/IFramebuffer.hpp"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class OpenGLFramebuffer final : public IFramebuffer
{
public:
  explicit OpenGLFramebuffer(const FramebufferConfiguration &configuration);
  virtual ~OpenGLFramebuffer();// NOLINT

  void Bind() override;
  void Unbind() override;

  auto ReadPixel(uint32_t attachmentIndex, const Position2DI &pixel) -> int override;
  void ClearAttachment(uint32_t attachmentIndex, int value) override;
  [[nodiscard]] inline auto AttachmentRendererID(uint32_t attachmentIndex = 0) const -> uint32_t override
  {
    ASSERT(attachmentIndex < m_AttachmentRendererIDs.size(), "Attachment doesn't exist");
    return m_AttachmentRendererIDs[attachmentIndex];
  }

private:
  void ResizeFramebuffer(const Size2DI &size) override;
  void
    AttachTexture(uint32_t attachmentSlot, FramebufferAttachmentFormat format, const Size2DI &size, uint32_t samples);

  uint32_t m_RendererID = 0;
  Vector<uint32_t, MemoryTag::Renderer> m_AttachmentRendererIDs;
};

}// namespace JE