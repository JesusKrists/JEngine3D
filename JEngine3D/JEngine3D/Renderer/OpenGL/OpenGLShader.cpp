#include "OpenGLShader.hpp"

#include "JEngine3D/Core/LoggerController.hpp"

#include <GL/glew.h>
#include <array>

#include <glm/gtc/type_ptr.hpp>

#include <Tracy.hpp>

namespace JE {

static void CheckShaderErrors(uint32_t rendererID, GLenum status)
{
  // print compile errors if any
  int32_t success = 0;
  if (status == GL_LINK_STATUS) {
    glGetProgramiv(rendererID, status, &success);
  } else {
    glGetShaderiv(rendererID, status, &success);
  }
  if (success == 0) {
    std::array<char, 512> infoLog{};// NOLINT
    glGetShaderInfoLog(rendererID, infoLog.size(), nullptr, infoLog.data());
    Logger::CoreLogger().error("Failed to compile/link shader: {}", infoLog.data());
    DEBUGBREAK();
  }
}

static auto GetUniformLocation(uint32_t rendererID, const std::string_view &uniformName) -> int32_t
{
  auto location = glGetUniformLocation(rendererID, uniformName.data());
  if (location == -1) { Logger::CoreLogger().error("Uniform location not found: {}", uniformName.data()); }

  return location;
}

static auto ShaderTypeToOpenGLShaderType(IShader::Type type) -> GLuint
{
  switch (type) {
  case IShader::Type::VERTEX:
    return GL_VERTEX_SHADER;
  case IShader::Type::FRAGMENT:
    return GL_FRAGMENT_SHADER;
  default:
    DEBUGBREAK();
    return 0;
  }
}

OpenGLShader::OpenGLShader(const std::string_view &name,// NOLINT
  const std::string_view &vertexSource,
  const std::string_view &fragmentSource)
  : m_Name(name), m_RendererID(glCreateProgram())
{
  ZoneScopedN("OpenGLShader::OpenGLShader");// NOLINT
  auto vertexShader = CreateShader(Type::VERTEX, vertexSource);
  auto fragmentShader = CreateShader(Type::FRAGMENT, fragmentSource);

  glAttachShader(m_RendererID, vertexShader);
  glAttachShader(m_RendererID, fragmentShader);
  glLinkProgram(m_RendererID);
  CheckShaderErrors(m_RendererID, GL_LINK_STATUS);

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

OpenGLShader::~OpenGLShader()
{
  ZoneScopedN("OpenGLShader::~OpenGLShader");// NOLINT
  glDeleteProgram(m_RendererID);
}

void OpenGLShader::Bind() const
{
  ZoneScopedN("OpenGLShader::Bind");// NOLINT
  glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const
{
  ZoneScopedN("OpenGLShader::Unbind");// NOLINT
  glUseProgram(0);
}


void OpenGLShader::SetInt(const std::string_view &name, int value)
{
  ZoneScopedN("OpenGLShader::SetInt");// NOLINT
  auto location = GetUniformLocation(m_RendererID, name);
  glUniform1i(location, value);
}

void OpenGLShader::SetMat4(const std::string_view &name, const glm::mat4 &value)
{
  ZoneScopedN("OpenGLShader::SetMat4");// NOLINT
  auto location = GetUniformLocation(m_RendererID, name);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

auto OpenGLShader::CreateShader(Type shaderType, const std::string_view &shaderSource) -> uint32_t
{
  auto shaderSourceStr = std::string(shaderSource);
  const auto *shaderSourcePtr = shaderSourceStr.c_str();

  auto shader = glCreateShader(ShaderTypeToOpenGLShaderType(shaderType));
  glShaderSource(shader, 1, &shaderSourcePtr, nullptr);
  glCompileShader(shader);
  CheckShaderErrors(shader, GL_COMPILE_STATUS);

  return shader;
}

}// namespace JE