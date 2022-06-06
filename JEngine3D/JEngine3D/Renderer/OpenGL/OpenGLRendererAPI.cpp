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
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(OpenGLMessageCallback, nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
}

void OpenGLRendererAPI::SetViewport(const RectangleI &viewport)
{
  glViewport(viewport.Position.X, viewport.Position.Y, viewport.Size.Width, viewport.Size.Height);
}

void OpenGLRendererAPI::SetClearColor(const Color &color)
{
  glClearColor(color.RawColor.r, color.RawColor.g, color.RawColor.b, color.RawColor.a);// NOLINT
}

void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }// NOLINT

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