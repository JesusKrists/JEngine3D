#pragma once

#include "JEngine3D/Renderer/IShader.hpp"

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class OpenGLShader final : public IShader
{
public:
  OpenGLShader(const std::string_view &name,
    const std::string_view &vertexSource,
    const std::string_view &fragmentSource);
  virtual ~OpenGLShader();// NOLINT

  void Bind() const override;
  void Unbind() const override;

  void SetInt(const std::string_view &name, int value) override;
  void SetMat4(const std::string_view &name, const glm::mat4 &value) override;

  [[nodiscard]] inline auto Name() const -> const std::string & override { return m_Name; }

private:
  static auto CreateShader(Type shaderType, const std::string_view &shaderSource) -> uint32_t;

  std::string m_Name;
  uint32_t m_RendererID = 0;
};

}// namespace JE