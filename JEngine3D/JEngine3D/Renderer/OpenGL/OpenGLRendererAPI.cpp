#include "OpenGLRendererAPI.hpp"

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Renderer/IVertexArray.hpp"

#include <GL/glew.h>

namespace JE {

static void OpenGLMessageCallback(unsigned source,// NOLINT
  unsigned type,// NOLINT
  unsigned id,// NOLINT
  unsigned severity,// NOLINT
  int length,// NOLINT
  const char *message,// NOLINT
  const void *userParam)// NOLINT
{

  UNUSED(source);
  UNUSED(type);
  UNUSED(id);
  UNUSED(length);
  UNUSED(userParam);

  switch (severity) {// NOLINT
  case GL_DEBUG_SEVERITY_HIGH:
    JE::Logger::CoreLogger().critical(message);
    return;
  case GL_DEBUG_SEVERITY_MEDIUM:
    JE::Logger::CoreLogger().error(message);
    return;
  case GL_DEBUG_SEVERITY_LOW:
    JE::Logger::CoreLogger().warn(message);
    return;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    JE::Logger::CoreLogger().trace(message);
    return;
  }

  DEBUGBREAK();
}

OpenGLRendererAPI::OpenGLRendererAPI()
{
  ZoneScopedN("OpenGLRendererAPI::OpenGLRendererAPI");// NOLINT
#if defined(JE_DEBUG)
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(OpenGLMessageCallback, nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

  UpdateRendererState();
}

void OpenGLRendererAPI::SetViewport(const RectangleI &viewport)
{
  ZoneScopedN("OpenGLRendererAPI::SetViewport");// NOLINT
  glViewport(viewport.Position.X, viewport.Position.Y, viewport.Size.Width, viewport.Size.Height);
  if (RendererState().ClipTest) {
    glScissor(viewport.Position.X, viewport.Position.Y, viewport.Size.Width, viewport.Size.Height);
  }
}

void OpenGLRendererAPI::SetClipRect(const RectangleI &clipRect)
{
  ZoneScopedN("OpenGLRendererAPI::SetClipRect");// NOLINT
  glScissor(clipRect.Position.X, clipRect.Position.Y, clipRect.Size.Width, clipRect.Size.Height);
}

void OpenGLRendererAPI::SetClearColor(const Color &color)
{
  ZoneScopedN("OpenGLRendererAPI::SetClearColor");// NOLINT
  glClearColor(color.RawColor.r, color.RawColor.g, color.RawColor.b, color.RawColor.a);// NOLINT
}

void OpenGLRendererAPI::Clear()
{
  ZoneScopedN("OpenGLRendererAPI::Clear");// NOLINT
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// NOLINT
}

void OpenGLRendererAPI::BindTexture(uint32_t rendererID, uint8_t slot)// NOLINT
{
  ZoneScopedN("OpenGLRendererAPI::BindTexture");// NOLINT
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, rendererID);
}

void OpenGLRendererAPI::DrawIndexed(const IVertexArray &vertexArray, size_t indexCount)
{
  ZoneScopedN("OpenGLRendererAPI::DrawIndexed");// NOLINT

  vertexArray.Bind();
  glDrawElements(GL_TRIANGLES,
    static_cast<GLsizei>(indexCount != 0 ? indexCount : vertexArray.IndexBuffer().IndexCount()),
    GL_UNSIGNED_INT,
    nullptr);
  vertexArray.Unbind();
}


void OpenGLRendererAPI::DrawIndexed(const IVertexArray &vertexArray, size_t indexCount, size_t indexOffset)
{
  ZoneScopedN("OpenGLRendererAPI::DrawIndexed(offset)");// NOLINT

  vertexArray.Bind();
  glDrawElements(GL_TRIANGLES,
    static_cast<GLsizei>(indexCount),
    GL_UNSIGNED_INT,
    reinterpret_cast<const void *>(indexOffset * sizeof(uint32_t)));// NOLINT
  vertexArray.Unbind();
}

void OpenGLRendererAPI::UpdateRendererState()
{
  ZoneScopedN("OpenGLRendererAPI::UpdateRendererState");// NOLINT
  if (RendererState().DepthTest) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }

  if (RendererState().ClipTest) {
    glEnable(GL_SCISSOR_TEST);
  } else {
    glDisable(GL_SCISSOR_TEST);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

}// namespace JE