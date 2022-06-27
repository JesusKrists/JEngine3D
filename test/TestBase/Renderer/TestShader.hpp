#pragma once

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Renderer/IShader.hpp>

class TestShader final : public JE::IShader
{
public:
  TestShader(const std::string_view &name,// NOLINT
    const std::string_view &vertexSource,
    const std::string_view &fragmentSource)
    : m_Name(name)
  {
    JE::UNUSED(vertexSource);
    JE::UNUSED(fragmentSource);
  }

  inline void Bind() const override {}
  inline void Unbind() const override {}

  inline void SetInt(const std::string_view &name, int value) override
  {
    JE::UNUSED(name);
    JE::UNUSED(value);
  }
  inline void SetMat4(const std::string_view &name, const glm::mat4 &value) override
  {
    JE::UNUSED(name);
    JE::UNUSED(value);
  }

  [[nodiscard]] inline auto Name() const -> const std::string & override { return m_Name; }

private:
  std::string m_Name;
};