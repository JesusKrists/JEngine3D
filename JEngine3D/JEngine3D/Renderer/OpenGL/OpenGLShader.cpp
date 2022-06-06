#include "OpenGLShader.hpp"

#include "JEngine3D/Core/Base.hpp"
#include "JEngine3D/Core/LoggerController.hpp"

#include <GL/glew.h>
#include <array>

namespace JE {

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
  auto vertexShader = CreateShader(Type::VERTEX, vertexSource);
  auto fragmentShader = CreateShader(Type::FRAGMENT, fragmentSource);

  glAttachShader(m_RendererID, vertexShader);
  glAttachShader(m_RendererID, fragmentShader);
  glLinkProgram(m_RendererID);

  // print linking errors if any
  int32_t success = 0;
  glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
  if (success == 0) {
    std::array<char, 512> infoLog{};// NOLINT
    glGetShaderInfoLog(m_RendererID, infoLog.size(), nullptr, infoLog.data());
    Logger::CoreLogger().error("Failed to link shader: {}", infoLog.data());
    DEBUGBREAK();
  };

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_RendererID); }

void OpenGLShader::Bind() const { glUseProgram(m_RendererID); }

void OpenGLShader::Unbind() const { glUseProgram(0); }

auto OpenGLShader::CreateShader(Type shaderType, const std::string_view &shaderSource) -> uint32_t
{
  auto shaderSourceStr = std::string(shaderSource);
  const auto *shaderSourcePtr = shaderSourceStr.c_str();

  auto shader = glCreateShader(ShaderTypeToOpenGLShaderType(shaderType));
  glShaderSource(shader, 1, &shaderSourcePtr, nullptr);
  glCompileShader(shader);

  // print compile errors if any
  int32_t success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == 0) {
    std::array<char, 512> infoLog{};// NOLINT
    glGetShaderInfoLog(shader, infoLog.size(), nullptr, infoLog.data());
    Logger::CoreLogger().error("Failed to compile shader: {}", infoLog.data());
    DEBUGBREAK();
  };

  return shader;
}

}// namespace JE