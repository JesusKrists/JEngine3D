#pragma once

#include <cstdint>
#include <cstddef>
#include <new>

auto operator new(size_t size) -> void*;// NOLINT
auto operator new[](size_t size) -> void*;// NOLINT
void operator delete(void* p) noexcept;// NOLINT
void operator delete(void* p, std::size_t) noexcept;
void operator delete[](void* p) noexcept;// NOLINT
void operator delete[](void* p, std::size_t) noexcept;

namespace JE {

    class NewOverrideDebug
    {
        friend auto ::operator new(size_t) -> void*;
        friend auto ::operator new[](size_t) -> void*;
        friend void ::operator delete(void*) noexcept;
        friend void ::operator delete(void*, std::size_t) noexcept;
        friend void ::operator delete[](void*) noexcept;
        friend void ::operator delete[](void*, std::size_t) noexcept;

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

        static NewOverrideDebug s_OverrideDebugInstance;// NOLINT
    };

}// namespace JE