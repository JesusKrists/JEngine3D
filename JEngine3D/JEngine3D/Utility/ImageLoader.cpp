#include "ImageLoader.hpp"

#include <JEngine3D/Core/Base.hpp>
#include <JEngine3D/Core/Assert.hpp>

#include <lunasvg.h>

namespace JE {

auto ImageLoader::LoadImageFromPath(const std::filesystem::path &filePath, const ImageConfig &config) -> Image
{
  auto fileExtension = filePath.extension();

  if (fileExtension.native() == ".svg") {
    ASSERT(config.SizePreference.Width != 0 && config.SizePreference.Height != 0, "SVG requires a size preference");

    auto document = lunasvg::Document::loadFromFile(filePath.native());
    auto bitmap = document->renderToBitmap(static_cast<std::uint32_t>(config.SizePreference.Width),
      static_cast<std::uint32_t>(config.SizePreference.Height));
    bitmap.convertToRGBA();

    return Image{ { reinterpret_cast<std::byte *>(bitmap.data()),// NOLINT
                    reinterpret_cast<std::byte *>(bitmap.data()) + (bitmap.width() * bitmap.height() * 4) },// NOLINT
      { static_cast<int32_t>(bitmap.width()), static_cast<int32_t>(bitmap.height()) },
      ImageFormat::RGBA8 };
  }

  DEBUGBREAK();
  return Image{};
}

}// namespace JE