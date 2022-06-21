#pragma once

#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Core/Types.hpp"

#include <JEngine3D/Renderer/ITexture.hpp>

#include <filesystem>

namespace JE {

enum class ImageFormat { RGBA8 };

inline auto ImageFormatToTextureFormat(ImageFormat format) -> TextureFormat
{
  switch (format) {
  case ImageFormat::RGBA8:
    return TextureFormat::RGBA8;

  default:
    DEBUGBREAK();
    return TextureFormat::RGBA8;
  }
}

struct ImageConfig
{
  Size2DI SizePreference = { 0, 0 };
  ImageFormat Format = ImageFormat::RGBA8;
};

struct Image
{
  std::string SourcePath;
  Vector<std::byte, MemoryTag::App> Data;
  Size2DI Size = { 0, 0 };
  ImageFormat Format = ImageFormat::RGBA8;
};

class ImageLoader
{
public:
  static auto LoadImageFromPath(const std::filesystem::path &filePath, const ImageConfig &config = ImageConfig())
    -> Image;
};

}// namespace JE