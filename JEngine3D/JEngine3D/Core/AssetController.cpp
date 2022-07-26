#include "AssetController.hpp"

#include "JEngine3D/Renderer/IRendererObjectCreator.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"
#include "JEngine3D/Utility/StringManipulation.hpp"
#include "JEngine3D/Utility/ImageLoader.hpp"

namespace JE {

    static const std::unordered_map<AssetType, std::vector<std::string>> EXTENSION_ASSET_TYPE_MAP = {// NOLINT
        { AssetType::TEXTURE, { ".png", ".jpg", ".svg" } },
        { AssetType::MESH, { ".obj" } }
    };


    static auto AssetTypeFromPath(const std::filesystem::path& filePath) -> AssetType
    {
        auto fileExtension = ToLower(filePath.extension().generic_string());

        for (int i = 0; i < static_cast<int>(AssetType::UNKNOWN); i++) {
            auto type                  = static_cast<AssetType>(i);
            auto typeExtensionVectorIt = EXTENSION_ASSET_TYPE_MAP.find(type);
            if (typeExtensionVectorIt != std::end(EXTENSION_ASSET_TYPE_MAP)) {
                auto extensionIt =
                FindIf(typeExtensionVectorIt->second, [&](const std::string& extension) { return extension == fileExtension; });
                if (extensionIt != std::end(typeExtensionVectorIt->second)) { return type; }
            }
        }

        return AssetType::UNKNOWN;
    }

    JAPI AssetController* AssetController::s_AssetControllerInstance = nullptr;// NOLINT

    AssetController::AssetController()
    {
        ASSERT(!s_AssetControllerInstance, "WindowController instance already exists");
        s_AssetControllerInstance = this;
    }

    AssetController::~AssetController() { s_AssetControllerInstance = nullptr; }


    auto AssetController::LoadAsset(const std::filesystem::path& filePath) -> AssetHandle*
    {
        auto assetIt =
        FindIf(m_AssetHandles, [&](const Scope<AssetHandle, MemoryTag::App>& assetHandle) { return assetHandle->m_AssetPath == filePath; });
        if (assetIt != std::end(m_AssetHandles)) { return assetIt->get(); }

        m_AssetHandles.push_back(CreateScope<AssetHandle, MemoryTag::App>(filePath.generic_string(), AssetTypeFromPath(filePath)));
        auto& handle = m_AssetHandles.back();
        LoadAsset(*handle);
        return handle.get();
    }

    void AssetController::LoadAsset(AssetHandle& handle)
    {
        switch (handle.m_AssetType) {
            case AssetType::TEXTURE: {
                auto image = ImageLoader::LoadImageFromPath(handle.m_AssetPath);
                m_TextureAssets.push_back(IRendererObjectCreator::Get().CreateTexture(image));
                auto* textureAsset = m_TextureAssets.back().get();
                handle.SetAssetPtr(textureAsset);
                handle.SetAssetLoaded();
            } break;

            default: break;
        }
    }

}// namespace JE