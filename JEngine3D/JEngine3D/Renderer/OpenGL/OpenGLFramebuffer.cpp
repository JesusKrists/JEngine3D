#include "OpenGLFramebuffer.hpp"

#include "JEngine3D/Core/Application.hpp"

#include <GL/glew.h>

#include <JEngine3D/Platform/IGraphicsContext.hpp>
#include <JEngine3D/Renderer/IFramebuffer.hpp>
#include <Tracy.hpp>

namespace JE {

static auto AttachmentFormatToOpenGLInternalFormat(FramebufferAttachmentFormat format) -> GLint
{
  switch (format) {
  case FramebufferAttachmentFormat::RGBA8:
    return GL_RGBA8;
  case FramebufferAttachmentFormat::RED_INTEGER:
    return GL_R32I;
  case FramebufferAttachmentFormat::DEPTH24STENCIL8:
    return GL_DEPTH24_STENCIL8;

  default:
    DEBUGBREAK();
    return 0;
  }
}

static auto AttachmentFormatToOpenGLDataFormat(FramebufferAttachmentFormat format) -> GLenum
{
  switch (format) {
  case FramebufferAttachmentFormat::RGBA8:
    return GL_RGBA;
  case FramebufferAttachmentFormat::RED_INTEGER:
    return GL_RED_INTEGER;
  case FramebufferAttachmentFormat::DEPTH24STENCIL8:
    return GL_DEPTH_STENCIL;

  default:
    DEBUGBREAK();
    return 0;
  }
}


OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferConfiguration &configuration) : IFramebuffer(configuration)
{
  ZoneScopedN("OpenGLFramebuffer::OpenGLFramebuffer");// NOLINT
  glGenFramebuffers(1, &m_RendererID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

  int colorBufferCount = 0;
  for (size_t i = 0; i < configuration.Attachments.size(); i++) {
    auto attachmentFormat = configuration.Attachments[i];
    if (attachmentFormat != FramebufferAttachmentFormat::DEPTH24STENCIL8) { colorBufferCount++; }
    AttachTexture(static_cast<uint32_t>(i), attachmentFormat, configuration.Size, configuration.Samples);
  }

  if (colorBufferCount != 0) {
    std::array<GLenum, 4> buffers = {
      GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
    };
    glDrawBuffers(colorBufferCount, buffers.data());// NOLINT

  } else {
    glDrawBuffer(GL_NONE);
  }

  ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


OpenGLFramebuffer::~OpenGLFramebuffer()
{
  glDeleteFramebuffers(1, &m_RendererID);
  glDeleteTextures(static_cast<GLsizei>(m_AttachmentRendererIDs.size()), m_AttachmentRendererIDs.data());
}

void OpenGLFramebuffer::Bind()
{
  ZoneScopedN("OpenGLFramebuffer::Bind");// NOLINT
  glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
  JE_APP.RendererAPI().SetViewport({ { 0, 0 }, { Configuration().Size.Width, Configuration().Size.Height } });
}

void OpenGLFramebuffer::Unbind()
{
  ZoneScopedN("OpenGLFramebuffer::Unbind");// NOLINT
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  IGraphicsContext::CurrentContext()->ResetViewport();
}

auto OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, const Position2DI &pixel) -> int
{
  ZoneScopedN("OpenGLFramebuffer::ReadPixel");// NOLINT

  ASSERT(attachmentIndex < m_AttachmentRendererIDs.size(), "Attachment doesn't exist");

  glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
  int pixelData = 0;
  glReadPixels(
    pixel.X, pixel.Y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);// This probably only works for GL_RED_INTEGER buffers
  return pixelData;
}

void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)// NOLINT
{
  ZoneScopedN("OpenGLFramebuffer::ClearAttachment");// NOLINT

  ASSERT(false, "Not implemented!");
  UNUSED(attachmentIndex);
  UNUSED(value);
}

void OpenGLFramebuffer::ResizeFramebuffer(const Size2DI &size)
{
  ZoneScopedN("OpenGLFramebuffer::ResizeFramebuffer");// NOLINT

  for (size_t i = 0; i < m_AttachmentRendererIDs.size(); i++) {
    auto attachmentFormat = Configuration().Attachments[i];

    glBindTexture(GL_TEXTURE_2D, m_AttachmentRendererIDs[i]);

    if (attachmentFormat != FramebufferAttachmentFormat::DEPTH24STENCIL8) {
      glTexImage2D(GL_TEXTURE_2D,
        0,
        AttachmentFormatToOpenGLInternalFormat(attachmentFormat),
        size.Width,
        size.Height,
        0,
        AttachmentFormatToOpenGLDataFormat(attachmentFormat),
        GL_UNSIGNED_BYTE,
        nullptr);
    } else {
      glTexImage2D(GL_TEXTURE_2D,
        0,
        AttachmentFormatToOpenGLInternalFormat(attachmentFormat),
        size.Width,
        size.Height,
        0,
        AttachmentFormatToOpenGLDataFormat(attachmentFormat),
        GL_UNSIGNED_INT_24_8,
        nullptr);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
  }
}


void OpenGLFramebuffer::AttachTexture(uint32_t attachmentSlot,
  FramebufferAttachmentFormat format,
  const Size2DI &size,
  uint32_t samples)
{
  UNUSED(samples);// TODO(JesusKrists): Add multisampled textures

  ZoneScopedN("OpenGLFramebuffer::AttachTexture");// NOLINT

  // generate texture
  uint32_t attachmentTexture = 0;
  glGenTextures(1, &attachmentTexture);
  glBindTexture(GL_TEXTURE_2D, attachmentTexture);

  if (format != FramebufferAttachmentFormat::DEPTH24STENCIL8) {
    glTexImage2D(GL_TEXTURE_2D,
      0,
      AttachmentFormatToOpenGLInternalFormat(format),
      size.Width,
      size.Height,
      0,
      AttachmentFormatToOpenGLDataFormat(format),
      GL_UNSIGNED_BYTE,
      nullptr);
  } else {
    glTexImage2D(GL_TEXTURE_2D,
      0,
      AttachmentFormatToOpenGLInternalFormat(format),
      size.Width,
      size.Height,
      0,
      AttachmentFormatToOpenGLDataFormat(format),
      GL_UNSIGNED_INT_24_8,
      nullptr);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);


  // attach it to currently bound framebuffer object
  if (format != FramebufferAttachmentFormat::DEPTH24STENCIL8) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentSlot, GL_TEXTURE_2D, attachmentTexture, 0);
  } else {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attachmentTexture, 0);
  }

  m_AttachmentRendererIDs.emplace_back(attachmentTexture);
}

}// namespace JE