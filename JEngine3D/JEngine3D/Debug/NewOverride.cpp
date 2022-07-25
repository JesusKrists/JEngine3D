#include <JEngine3D/Debug/NewOverrideDebug.hpp>

#include <cstdlib>

void* operator new(size_t size)// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalAllocCount++;
    debugInstance.m_FrameGlobalAllocCounter++;
    return std::malloc(size);// NOLINT
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalAllocCount++;
    debugInstance.m_FrameGlobalAllocCounter++;
    return std::malloc(size);// NOLINT
}

void* operator new[](size_t size)// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalAllocCount++;
    debugInstance.m_FrameGlobalAllocCounter++;
    return std::malloc(size);// NOLINT
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalAllocCount++;
    debugInstance.m_FrameGlobalAllocCounter++;
    return std::malloc(size);// NOLINT
}

void operator delete(void* p) noexcept// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalDeallocCount++;
    debugInstance.m_FrameGlobalDeallocCounter++;
    return std::free(p);// NOLINT
}

void operator delete(void* p, std::size_t) noexcept// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalDeallocCount++;
    debugInstance.m_FrameGlobalDeallocCounter++;
    return std::free(p);// NOLINT
}

void operator delete[](void* p) noexcept// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalDeallocCount++;
    debugInstance.m_FrameGlobalDeallocCounter++;
    return std::free(p);// NOLINT
}

void operator delete[](void* p, std::size_t) noexcept// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalDeallocCount++;
    debugInstance.m_FrameGlobalDeallocCounter++;
    return std::free(p);// NOLINT
}


void* operator new(std::size_t size, std::align_val_t)// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalAllocCount++;
    debugInstance.m_FrameGlobalAllocCounter++;
    return std::malloc(size);// NOLINT
}

void* operator new(std::size_t size, std::align_val_t, const std::nothrow_t&) noexcept// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalAllocCount++;
    debugInstance.m_FrameGlobalAllocCounter++;
    return std::malloc(size);// NOLINT
}

void* operator new[](std::size_t size, std::align_val_t)// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalAllocCount++;
    debugInstance.m_FrameGlobalAllocCounter++;
    return std::malloc(size);// NOLINT
}

void* operator new[](std::size_t size, std::align_val_t, const std::nothrow_t&) noexcept// NOLINT
{
    auto& debugInstance = JE::NewOverrideDebug::Get();
    debugInstance.m_GlobalAllocCount++;
    debugInstance.m_FrameGlobalAllocCounter++;
    return std::malloc(size);// NOLINT
}