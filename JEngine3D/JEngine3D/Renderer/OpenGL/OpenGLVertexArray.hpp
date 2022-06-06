#pragma once

#include "JEngine3D/Renderer/IVertexArray.hpp"

namespace JE {

class OpenGLVertexArray : public IVertexArray
{
public:
  void Bind() const override {}
  void Unbind() const override {}

  void ConfigureVertexBufferLayout(const IVertexBuffer &vertexBuffer) override { UNUSED(vertexBuffer); }
};

}// namespace JE