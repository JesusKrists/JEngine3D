#include "ImageLoader.hpp"

#include <JEngine3D/Core/Assert.hpp>
#include <JEngine3D/Utility/StringManipulation.hpp>

#include <stb_image.h>
#include <lunasvg.h>

namespace JE {

    auto ImageLoader::LoadImageFromPath(const std::filesystem::path& filePath, const ImageConfig& config) -> Image
    {
        auto       fileExtension = ToLower(filePath.extension().generic_string());
        const auto FILE_PATH     = filePath.generic_string();

        if (fileExtension == SVG_EXTENSION_STR) {
            ASSERT(config.SizePreference.Width != 0 && config.SizePreference.Height != 0, "SVG requires a size preference");


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

        if (fileExtension == PNG_EXTENSION_STR || fileExtension == JPG_EXTENSION_STR) {
            stbi_set_flip_vertically_on_load(1);
            JE::Size2DI    imageSize     = { 0, 0 };
            int            imageChannels = 0;
            unsigned char* data          = stbi_load(FILE_PATH.c_str(), &imageSize.Width, &imageSize.Height, &imageChannels, 4);

            auto image       = Image{};
            image.SourcePath = FILE_PATH;
            image.Data.assign(reinterpret_cast<std::byte*>(data),// NOLINT
                              reinterpret_cast<std::byte*>(data) + (imageSize.Width * imageSize.Height * 4));// NOLINT
            image.Size = imageSize;

            stbi_image_free(data);

            return image;
        }


        DEBUGBREAK();
        return Image{};
    }

}// namespace JE