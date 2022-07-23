#pragma once

#include <glm/glm.hpp>
#include <JEngine3D/Core/Types.hpp>

#define ImDrawIdx unsigned int
#define ImTextureID unsigned int
#define IMGUI_DEFINE_MATH_OPERATORS 1

#define IM_VEC2_CLASS_EXTRA                                                                                                                \
    ImVec2(const JE::Size2DI& size) : x(static_cast<float>(size.Width)), y(static_cast<float>(size.Height))                                \
    {}                                                                                                                                     \
    inline operator JE::Size2DI() const                                                                                                    \
    {                                                                                                                                      \
        return JE::Size2DI{ static_cast<int32_t>(x), static_cast<int32_t>(y) };                                                            \
    }                                                                                                                                      \
    ImVec2(const JE::Position2DI& pos) : x(static_cast<float>(pos.X)), y(static_cast<float>(pos.Y))                                        \
    {}                                                                                                                                     \
    inline operator JE::Position2DI() const                                                                                                \
    {                                                                                                                                      \
        return JE::Position2DI{ static_cast<int32_t>(x), static_cast<int32_t>(y) };                                                        \
    }                                                                                                                                      \
    ImVec2(const glm::vec2& vec) : x(vec.x), y(vec.y)                                                                                      \
    {}                                                                                                                                     \
    inline operator glm::vec2() const                                                                                                      \
    {                                                                                                                                      \
        return glm::vec2{ x, y };                                                                                                          \
    }


#define IM_VEC4_CLASS_EXTRA                                                                                                                \
    ImVec4(const JE::Color& color) : x(color.RawColor.x), y(color.RawColor.y), z(color.RawColor.z), w(color.RawColor.w)                    \
    {}                                                                                                                                     \
    inline operator JE::Color() const                                                                                                      \
    {                                                                                                                                      \
        return JE::Color{ x, y, z, w };                                                                                                    \
    }                                                                                                                                      \
    ImVec4(const glm::vec4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w)                                                                  \
    {}                                                                                                                                     \
    inline operator glm::vec4() const                                                                                                      \
    {                                                                                                                                      \
        return glm::vec4{ x, y, z, w };                                                                                                    \
    }
