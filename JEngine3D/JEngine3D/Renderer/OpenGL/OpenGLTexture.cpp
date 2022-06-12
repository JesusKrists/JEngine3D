#include "OpenGLTexture.hpp"

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"
#include "JEngine3D/Core/Application.hpp"

#include <GL/glew.h>

namespace JE {

static auto TextureFormatToOpenGLInternalFormat(TextureFormat format) -> GLint
{
  switch (format) {
  case TextureFormat::RGB8:
    return GL_RGB8;
  case TextureFormat::RGBA8:
    return GL_RGBA8;
  default:
    DEBUGBREAK();
    return 0;
  }
}

static auto TextureFormatToOpenGLDataFormat(TextureFormat format) -> GLenum
{
  switch (format) {
  case TextureFormat::RGB8:
    return GL_RGB;
  case TextureFormat::RGBA8:
    return GL_RGBA;

  default:
    DEBUGBREAK();
    return 0;
  }
}

OpenGLTexture2D::OpenGLTexture2D(const std::string_view &sourcePath,
  const std::span<const std::byte> &textureData,
  const Size2DI &textureDimensions,
  TextureFormat format)
  : m_SourcePath(sourcePath), m_TextureSize(textureDimensions), m_Format(format)
{
  glGenTextures(1, &m_RendererID);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);

  glTexImage2D(GL_TEXTURE_2D,
    0,
    TextureFormatToOpenGLInternalFormat(m_Format),
    m_TextureSize.Width,
    m_TextureSize.Height,
    0,
    TextureFormatToOpenGLDataFormat(m_Format),
    GL_UNSIGNED_BYTE,
    textureData.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLTexture2D::OpenGLTexture2D(TextureFormat format)
  : m_SourcePath("temporary"), m_TextureSize({ 0, 0 }), m_Format(format)
{

  glGenTextures(1, &m_RendererID);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);

  glTexImage2D(GL_TEXTURE_2D,
    0,
    TextureFormatToOpenGLInternalFormat(m_Format),
    m_TextureSize.Width,
    m_TextureSize.Height,
    0,
    TextureFormatToOpenGLDataFormat(m_Format),
    GL_UNSIGNED_BYTE,
    nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_RendererID); }

void OpenGLTexture2D::SetData(const std::string_view &sourcePath,
  const std::span<const std::byte> &textureData,
  const Size2DI &textureDimensions,
  TextureFormat format)
{
  m_SourcePath = sourcePath;
  m_TextureSize = textureDimensions;
  m_Format = format;

  glBindTexture(GL_TEXTURE_2D, m_RendererID);

  glTexImage2D(GL_TEXTURE_2D,
    0,
    TextureFormatToOpenGLInternalFormat(m_Format),
    m_TextureSize.Width,
    m_TextureSize.Height,
    0,
    TextureFormatToOpenGLDataFormat(m_Format),
    GL_UNSIGNED_BYTE,
    textureData.data());
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture2D::Bind(uint8_t slot) const { JE_APP.RendererAPI().BindTexture(m_RendererID, slot); }

}// namespace JE