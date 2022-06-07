#include "OpenGLRendererAPI.hpp"

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Renderer/IVertexArray.hpp"

#include <GL/glew.h>

namespace JE {

static bool s_ScissorEnabled = true;

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
#if defined(JE_DEBUG)
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(OpenGLMessageCallback, nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRendererAPI::SetViewport(const RectangleI &viewport)
{
  glViewport(viewport.Position.X, viewport.Position.Y, viewport.Size.Width, viewport.Size.Height);
  if (s_ScissorEnabled) {
    glScissor(viewport.Position.X, viewport.Position.Y, viewport.Size.Width, viewport.Size.Height);
  }
}

void OpenGLRendererAPI::SetClearColor(const Color &color)
{
  glClearColor(color.RawColor.r, color.RawColor.g, color.RawColor.b, color.RawColor.a);// NOLINT
}

void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }// NOLINT

void OpenGLRendererAPI::BindTexture(uint32_t rendererID, uint8_t slot)// NOLINT
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, rendererID);
}

void OpenGLRendererAPI::DrawIndexed(const IVertexArray &vertexArray, size_t indexCount)
{
  vertexArray.Bind();
  glDrawElements(GL_TRIANGLES,
    static_cast<int32_t>(indexCount != 0 ? indexCount : vertexArray.IndexBuffer().IndexCount()),
    GL_UNSIGNED_INT,
    nullptr);
  vertexArray.Unbind();
}

}// namespace JE