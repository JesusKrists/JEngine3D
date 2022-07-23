#pragma once

#include "JEngine3D/Core/Base.hpp"
#include <string>

#include <glm/glm.hpp>
namespace JE {

    inline auto TextureSamplerUniformString(uint8_t slot) -> std::string_view
    {
        switch (slot) {
            case 0: return "u_Texture[0]";
            case 1: return "u_Texture[1]";
            case 2: return "u_Texture[2]";
            case 3: return "u_Texture[3]";
            case 4: return "u_Texture[4]";
            case 5:// NOLINT
                return "u_Texture[5]";
            case 6:// NOLINT
                return "u_Texture[6]";
            case 7:// NOLINT
                return "u_Texture[7]";
            case 8:// NOLINT
                return "u_Texture[8]";
            case 9:// NOLINT
                return "u_Texture[9]";
            case 10:// NOLINT
                return "u_Texture[10]";
            case 11:// NOLINT
                return "u_Texture[11]";
            case 12:// NOLINT
                return "u_Texture[12]";
            case 13:// NOLINT
                return "u_Texture[13]";
            case 14:// NOLINT
                return "u_Texture[14]";
            case 15:// NOLINT
                return "u_Texture[15]";

            default: DEBUGBREAK(); return "INVALID TEXTURE SLOT";
        }
    }

    // NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
    class IShader
    {
    public:
        enum class Type { VERTEX, FRAGMENT };

        virtual ~IShader() = default;

        virtual void Bind() const   = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(const std::string_view& name, int value)               = 0;
        virtual void SetMat4(const std::string_view& name, const glm::mat4& value) = 0;

        [[nodiscard]] virtual auto Name() const -> const std::string& = 0;
    };

}// namespace JE