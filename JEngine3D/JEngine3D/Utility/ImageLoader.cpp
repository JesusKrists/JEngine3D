#include "ImageLoader.hpp"

#include <JEngine3D/Core/Assert.hpp>
#include <JEngine3D/Utility/StringManipulation.hpp>

#include <lunasvg.h>

namespace JE {

    auto ImageLoader::LoadImageFromPath(const std::filesystem::path& filePath, const ImageConfig& config) -> Image
    {
        auto fileExtension = ToLower(filePath.extension().generic_string());

        if (fileExtension == SVG_EXTENSION_STR) {
            ASSERT(config.SizePreference.Width != 0 && config.SizePreference.Height != 0, "SVG requires a size preference");

            const auto FILE_PATH = filePath.generic_string();

            auto document = lunasvg::Document::loadFromFile(FILE_PATH);
            auto bitmap   = document->renderToBitmap(static_cast<std::uint32_t>(config.SizePreference.Width),
                                                   static_cast<std::uint32_t>(config.SizePreference.Height));
            bitmap.convertToRGBA();

            return Image{ FILE_PATH,
                          { reinterpret_cast<std::byte*>(bitmap.data()),// NOLINT
                            reinterpret_cast<std::byte*>(bitmap.data()) + (bitmap.width() * bitmap.height() * 4) },// NOLINT
                          { static_cast<int32_t>(bitmap.width()), static_cast<int32_t>(bitmap.height()) },
                          ImageFormat::RGBA8 };
        }

        DEBUGBREAK();
        return Image{};
    }

}// namespace JE