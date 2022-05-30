#pragma once

#include "JEngine3D/Core/Types.hpp"
#include "JEngine3D/Renderer/IShader.hpp"
#include "JEngine3D/Renderer/Software/SoftwareRendererAPI.hpp"
#include "JEngine3D/Renderer/Software/SoftwareTexture.hpp"

#include <glm/glm.hpp>

namespace JE {

class ISoftwareShader : public IShader
{
public:
  inline void Bind() override { SoftwareRendererAPI::BindShader(this); }
  inline void Unbind() override { SoftwareRendererAPI::BindShader(nullptr); }

  virtual auto VertexShader(const Vertex &vertex, uint32_t index) -> glm::vec4 = 0;
  virtual auto FragmentShader(const glm::vec3 &barycentric, uint32_t &pixelColorOut) -> bool = 0;
};

inline auto CalculateColorFromBarycentric(const glm::vec3 &bary, uint32_t colorU, uint32_t colorV, uint32_t colorW)
  -> uint32_t
{
  uint8_t A = static_cast<uint8_t>(static_cast<float>((colorU >> 24) & 0xFF) * bary.x// NOLINT
                                   + static_cast<float>((colorV >> 24) & 0xFF) * bary.y// NOLINT
                                   + static_cast<float>((colorW >> 24) & 0xFF) * bary.z);// NOLINT

  uint8_t B = static_cast<uint8_t>(static_cast<float>((colorU >> 16) & 0xFF) * bary.x// NOLINT
                                   + static_cast<float>((colorV >> 16) & 0xFF) * bary.y// NOLINT
                                   + static_cast<float>((colorW >> 16) & 0xFF) * bary.z);// NOLINT

  uint8_t G = static_cast<uint8_t>(static_cast<float>((colorU >> 8) & 0xFF) * bary.x// NOLINT
                                   + static_cast<float>((colorV >> 8) & 0xFF) * bary.y// NOLINT
                                   + static_cast<float>((colorW >> 8) & 0xFF) * bary.z);// NOLINT

  uint8_t R = static_cast<uint8_t>(static_cast<float>(colorU & 0xFF) * bary.x// NOLINT
                                   + static_cast<float>(colorV & 0xFF) * bary.y// NOLINT
                                   + static_cast<float>(colorW & 0xFF) * bary.z);// NOLINT

  return static_cast<uint32_t>((A << 24) | (B << 16) | (G << 8) | R);// NOLINT
}

inline auto
  CalculateUVFromBarycentric(const glm::vec3 &bary, const glm::vec2 &UVu, const glm::vec2 &UVv, const glm::vec2 &UVw)
    -> glm::vec2
{
  return { UVu.x * bary.x + UVv.x * bary.y + UVw.x * bary.z,// NOLINT
    UVu.y * bary.x + UVv.y * bary.y + UVw.y * bary.z };// NOLINT
}

inline auto SampleTexture(const glm::vec2 &uv, const SoftwareTexture &texture) -> uint32_t
{
  const auto &textureSize = texture.Size();
  uint32_t X = static_cast<uint32_t>(uv.x * static_cast<float>(textureSize.Width - 1) + 0.5F);// NOLINT
  uint32_t Y = static_cast<uint32_t>(uv.y * static_cast<float>(textureSize.Height - 1) + 0.5F);// NOLINT

  return reinterpret_cast<const uint32_t *>(// NOLINT
    texture.PixelPtr())[X + Y * static_cast<uint32_t>(textureSize.Width)];
}

inline auto MultiplyColor(uint32_t color1, uint32_t color2) -> uint32_t
{
  return (((color1 >> 24) & 0xFF) * ((color2 >> 24) & 0xFF) / 255) << 24
         | (((color1 >> 16) & 0xFF) * ((color2 >> 16) & 0xFF) / 255) << 16
         | (((color1 >> 8) & 0xFF) * ((color2 >> 8) & 0xFF) / 255) << 8 | (((color1)&0xFF) * ((color2)&0xFF) / 255);
}

}// namespace JE