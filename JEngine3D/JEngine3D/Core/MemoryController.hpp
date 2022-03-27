#pragma once

#include "JEngine3D/Core/Assert.hpp"
#include "JEngine3D/Core/Base.hpp"

#include <cstddef>
#include <array>
#include <utility>
#include <vector>
#include <memory>

namespace JE {

enum class MemoryTag {
  Unknown,
  Logger,
  Graphics,
  App,

  TAG_COUNT
};

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class MemoryController
{
  static constexpr size_t ALIGNMENT = 4;

public:
  MemoryController();
  ~MemoryController();

  static inline auto Get() -> MemoryController &
  {
    ASSERT(s_MemoryControllerInstance, "MemoryController instance is null");
    return *s_MemoryControllerInstance;
  }

  template<typename T, MemoryTag TAG = MemoryTag::Unknown> inline auto Allocate(size_t elementCount = 1) -> T *
  {
    auto memorySize = AlignTo(sizeof(T) * elementCount, ALIGNMENT);// NOLINT
    return static_cast<T *>(Allocate(memorySize, TAG));
  }

  template<typename T, MemoryTag TAG = MemoryTag::Unknown> inline void Deallocate(T *memory)
  {
    return Deallocate(static_cast<void *>(memory), TAG);
  }


private:
  auto Allocate(size_t byteCount, MemoryTag tag) -> void *;
  void Deallocate(void *memory, MemoryTag tag);

  struct MemoryEntry
  {
    MemoryEntry(void *memory, size_t byteCount, MemoryTag tag) : Memory(memory), ByteCount(byteCount), Tag(tag) {}

    void *Memory;
    size_t ByteCount;
    MemoryTag Tag;
  };
  inline auto GetMemoryEntryContainer(MemoryTag tag) -> std::vector<MemoryEntry> &
  {
    return m_MemoryEntries.at(static_cast<size_t>(tag));
  }

  std::array<std::vector<MemoryEntry>, static_cast<size_t>(MemoryTag::TAG_COUNT)> m_MemoryEntries;

  static MemoryController *s_MemoryControllerInstance;// NOLINT
};

template<typename T, MemoryTag TAG = MemoryTag::Unknown> struct MemoryControllerAllocator : public std::allocator<T>
{
  template<class U> struct rebind
  {
    using other = MemoryControllerAllocator<U, TAG>;
  };

  inline void operator()(T *memory) const { Destruct(memory); }

  inline void Destruct(T *memory) const
  {
    memory->~T();
    MemoryController::Get().Deallocate<T, TAG>(memory);
  }

  [[nodiscard]] inline auto allocate(size_t count) -> T * { return MemoryController::Get().Allocate<T, TAG>(count); }

  inline void deallocate(T *memory, [[maybe_unused]] size_t count) noexcept
  {
    MemoryController::Get().Deallocate<T, TAG>(memory);
  }
};

template<typename T, MemoryTag TAG = MemoryTag::Unknown>
using Scope = std::unique_ptr<T, MemoryControllerAllocator<T, TAG>>;

template<typename T, MemoryTag TAG = MemoryTag::Unknown, typename... Args>
inline auto CreateScope(Args &&...args) -> Scope<T, TAG>
{
  return Scope<T, TAG>(new (MemoryController::Get().Allocate<T, TAG>(1)) T(std::forward<Args>(args)...));
}

template<typename T, MemoryTag TAG = MemoryTag::Unknown> using Ref = std::shared_ptr<T>;

template<typename T, MemoryTag TAG = MemoryTag::Unknown, typename... Args>
inline auto CreateRef(Args &&...args) -> Ref<T, TAG>
{
  return Ref<T, TAG>(new (MemoryController::Get().Allocate<T, TAG>(1)) T(std::forward<Args>(args)...),
    MemoryControllerAllocator<T, TAG>{});
}

template<typename T, MemoryTag TAG = MemoryTag::Unknown>
using Vector = std::vector<T, MemoryControllerAllocator<T, TAG>>;

}// namespace JE