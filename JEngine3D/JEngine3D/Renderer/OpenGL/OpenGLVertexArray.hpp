#pragma once

#include "JEngine3D/Renderer/IVertexArray.hpp"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class OpenGLVertexArray final : public IVertexArray
{
public:
  OpenGLVertexArray();
  virtual ~OpenGLVertexArray();// NOLINT

  void Bind() const override;
  void Unbind() const override;

  void ConfigureVertexBufferLayout(const BufferLayout &bufferLayout) override;

private:
  uint32_t m_RendererID = 0;
  uint32_t m_LocationIndex = 0;
};

}// namespace JE