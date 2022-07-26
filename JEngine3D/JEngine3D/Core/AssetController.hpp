#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/MemoryController.hpp"
#include "JEngine3D/Renderer/ITexture.hpp"

#include <filesystem>

namespace JE {

    enum class AssetType { TEXTURE, MESH, UNKNOWN };

    class AssetHandle
    {
        friend class AssetController;

    public:
        explicit AssetHandle(const std::string_view& assetPath, AssetType type) : m_AssetPath(assetPath), m_AssetType(type) {}

        [[nodiscard]] inline auto Loaded() const -> bool { return m_Loaded; }

        template<typename T>
        [[nodiscard]] inline auto As() -> T&
        {
            ASSERT(m_Loaded && m_AssetPtr != nullptr, "Asset not ready");
            return *reinterpret_cast<T*>(m_AssetPtr);// NOLINT
        }

    private:
        inline void SetAssetLoaded() { m_Loaded = true; }
        inline void SetAssetPtr(void* ptr) { m_AssetPtr = ptr; }

        std::string m_AssetPath;
        AssetType   m_AssetType;
        bool        m_Loaded   = false;
        void*       m_AssetPtr = nullptr;
    };

    // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
    class AssetController
    {
        using AssetHandleContainer = Vector<Scope<AssetHandle, MemoryTag::App>, MemoryTag::App>;

    public:
        AssetController();
        ~AssetController();

        [[nodiscard]] static inline auto Get() -> AssetController&
        {
            ASSERT(s_AssetControllerInstance, "AssetController instance is null");
            return *s_AssetControllerInstance;
        }

        auto LoadAsset(const std::filesystem::path& filePath) -> AssetHandle*;

    private:
        void LoadAsset(AssetHandle& handle);

        AssetHandleContainer                                           m_AssetHandles;
        Vector<Scope<ITexture2D, MemoryTag::Renderer>, MemoryTag::App> m_TextureAssets;

        JAPI static AssetController* s_AssetControllerInstance;// NOLINT
    };
}// namespace JE