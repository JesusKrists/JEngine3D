#pragma once

#include "JEngine3D/Renderer/IVertexArray.hpp"
#include "JEngine3D/Renderer/Software/SoftwareRendererAPI.hpp"

namespace JE {

class SoftwareVertexArray : public IVertexArray
{
public:
  inline void Bind() const override { SoftwareRendererAPI::BindVertexArray(this); }
  inline void Unbind() const override { SoftwareRendererAPI::BindVertexArray(nullptr); }

  inline void ConfigureVertexBufferLayout(const IVertexBuffer &vertexBuffer) override { UNUSED(vertexBuffer); }
};

}// namespace JE