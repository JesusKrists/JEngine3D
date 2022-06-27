#pragma once

#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Renderer/IDrawTarget.hpp"

#include <Tracy.hpp>

namespace JE {

enum class FramebufferAttachmentFormat {
  None = 0,

  // Color
  RGBA8,
  RED_INTEGER,

  // Depth/stencil
  DEPTH24STENCIL8
};

struct FramebufferConfiguration
{
  Size2DI Size;
  Vector<FramebufferAttachmentFormat, MemoryTag::Renderer> Attachments;
  uint32_t Samples = 1;
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class IFramebuffer : public IDrawTarget
{
public:
  explicit IFramebuffer(const FramebufferConfiguration &configuration) : m_Configuration(configuration) {}// NOLINT
  virtual ~IFramebuffer() = default;// NOLINT

  virtual auto ReadPixel(uint32_t attachmentIndex, const Position2DI &pixel) -> int = 0;
  virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
  [[nodiscard]] virtual auto AttachmentRendererID(uint32_t index = 0) const -> uint32_t = 0;

  inline void Resize(const Size2DI &size)
  {
    ZoneScopedN("IFrameBuffer::Resize");// NOLINT
    m_Configuration.Size = size;
    ResizeFramebuffer(size);
  }
  [[nodiscard]] inline auto Configuration() const -> const FramebufferConfiguration & { return m_Configuration; }

private:
  virtual void ResizeFramebuffer(const Size2DI &size) = 0;

  FramebufferConfiguration m_Configuration;
};


}// namespace JE