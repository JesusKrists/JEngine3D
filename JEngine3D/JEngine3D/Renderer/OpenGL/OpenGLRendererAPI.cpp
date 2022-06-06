#include "OpenGLRendererAPI.hpp"

#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Renderer/IVertexArray.hpp"

#include <GL/glew.h>

namespace JE {

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
    static_cast<int32_t>(indexCount != 0 ? indexCount : vertexArray.IndexBuffer().Count()),
    GL_UNSIGNED_INT,
    nullptr);
  vertexArray.Unbind();
}

}// namespace JE