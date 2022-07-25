#pragma once

#include "JEngine3D/Core/Base.hpp"

auto operator new(std::size_t) -> void*;// NOLINT
auto operator new(std::size_t, const std::nothrow_t&) noexcept -> void*;// NOLINT
auto operator new[](std::size_t) -> void*;// NOLINT
auto operator new[](std::size_t, const std::nothrow_t&) noexcept -> void*;// NOLINT

void operator delete(void*) noexcept;// NOLINT
void operator delete(void*, std::size_t) noexcept;// NOLINT
void operator delete[](void*) noexcept;// NOLINT
void operator delete[](void*, std::size_t) noexcept;// NOLINT

auto operator new(std::size_t, std::align_val_t) -> void*;// NOLINT
auto operator new(std::size_t, std::align_val_t, const std::nothrow_t&) noexcept -> void*;// NOLINT
auto operator new[](std::size_t, std::align_val_t) -> void*;// NOLINT
auto operator new[](std::size_t, std::align_val_t, const std::nothrow_t&) noexcept -> void*;// NOLINT


namespace JE {

    class NewOverrideDebug
    {
        friend auto ::operator new(std::size_t) -> void*;
        friend auto ::operator new[](std::size_t) -> void*;
        friend auto ::operator new(std::size_t, const std::nothrow_t&) noexcept -> void*;
        friend auto ::operator new[](std::size_t, const std::nothrow_t&) noexcept -> void*;

        friend void ::operator delete(void*) noexcept;
        friend void ::operator delete(void*, std::size_t) noexcept;
        friend void ::operator delete[](void*) noexcept;
        friend void ::operator delete[](void*, std::size_t) noexcept;


        friend auto ::operator new(std::size_t, std::align_val_t) -> void*;// NOLINT
        friend auto ::operator new(std::size_t, std::align_val_t, const std::nothrow_t&) noexcept -> void*;// NOLINT
        friend auto ::operator new[](std::size_t, std::align_val_t) -> void*;// NOLINT
        friend auto ::operator new[](std::size_t, std::align_val_t, const std::nothrow_t&) noexcept -> void*;// NOLINT

    public:
        NewOverrideDebug() = default;

        [[nodiscard]] static inline auto Get() -> NewOverrideDebug& { return s_OverrideDebugInstance; }

        inline void NewFrame()
        {
            m_FrameGlobalAllocCount   = m_FrameGlobalAllocCounter;
            m_FrameGlobalDeallocCount = m_FrameGlobalDeallocCounter;

            m_FrameGlobalAllocCounter   = 0;
            m_FrameGlobalDeallocCounter = 0;
        }

        [[nodiscard]] inline auto GlobalAllocCount() const -> uint64_t { return m_GlobalAllocCount; }
        [[nodiscard]] inline auto GlobalDeallocCount() const -> uint64_t { return m_GlobalDeallocCount; }
        [[nodiscard]] inline auto FrameGlobalAllocCount() const -> uint64_t { return m_FrameGlobalAllocCount; }
        [[nodiscard]] inline auto FrameGlobalDeallocCount() const -> uint64_t { return m_FrameGlobalDeallocCount; }

    private:
        uint64_t m_GlobalAllocCount   = 0;
        uint64_t m_GlobalDeallocCount = 0;

        uint64_t m_FrameGlobalAllocCounter   = 0;
        uint64_t m_FrameGlobalDeallocCounter = 0;
        uint64_t m_FrameGlobalAllocCount     = 0;
        uint64_t m_FrameGlobalDeallocCount   = 0;

        JAPI static NewOverrideDebug s_OverrideDebugInstance;// NOLINT
    };

}// namespace JE