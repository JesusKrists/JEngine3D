#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include "TestPlatformBackend.hpp"

#include <JEngine3D/Core/Base.hpp>// for UNUSED
#include <JEngine3D/Core/Assert.hpp>// for ASSERT_, ASSERT
#include <JEngine3D/Core/Types.hpp>// for Size2D
#include <JEngine3D/Platform/IPlatformBackend.hpp>// for IPlatformBackend

#include <memory>
#include <array>// for array
#include <cstdlib>// for size_t, free, malloc
#include <iostream>// for operator<<, basic...
#include <iterator>// for end
#include <new>// for operator new
#include <string>// for string
#include <string_view>// for string_view
#include <utility>// for forward
#include <vector>// for vector

namespace JE {

// TODO(JesusKrists): Write constexpr tests for JE::FindIf and JE::Size2D operator== and JE::AlignTo and
// JE::MemoryController and JE::Window

constexpr auto AlignTo(size_t value, size_t alignment) -> size_t
{
  if (value == 0) { return alignment; }

  auto remainder = value % alignment;
  if (remainder == 0) { return value; }

  return value + (alignment - remainder);
}

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
  MemoryController()
  {
    ASSERT(!s_MemoryControllerInstance, "MemoryController instance already created");
    s_MemoryControllerInstance = this;
  }
  ~MemoryController()
  {
    for (const auto &memoryTagEntries : m_MemoryEntries) {
      ASSERT(memoryTagEntries.empty(),
        "MemoryController still has allocated entries, there is a memory leak somewhere in the program!");
    }
  }

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
  auto Allocate(size_t byteCount, MemoryTag tag) -> void *
  {
    auto *memory = std::malloc(byteCount);// NOLINT
    ASSERT(memory, "Failed to allocate memory from the heap");

    GetMemoryEntryContainer(tag).emplace_back(memory, byteCount, tag);

    if (tag == MemoryTag::Unknown) {
      // spdlog::warn("Allocating unknown memory tag, reclass this allocation! - {} bytes @ 0x{}", byteCount, memory);
      // // TODO(JesusKrists) - Log this properly
      std::cout << "Allocating unknown memory tag, reclass this allocation! - " << byteCount << " bytes @ 0x" << memory
                << "\n";
    }

    return memory;
  }

  void Deallocate(void *memory, MemoryTag tag)
  {
    auto &memoryEntryContainer = GetMemoryEntryContainer(tag);

    auto memoryEntry =
      FindIf(memoryEntryContainer, [&memory](const MemoryEntry &entry) { return entry.Memory == memory; });

    ASSERT(memoryEntry != std::end(memoryEntryContainer),
      "Memory entry does not exist for this allocation, invalid deallocation");

    if (memoryEntry->Tag != tag) {
      // spdlog::critical("Deallocate: MemoryTag mismatch for memory 0x{}", memoryEntry->Memory); // TODO(JesusKrists) -
      // Log this properly
      std::cout << "Deallocate: MemoryTag mismatch for memory 0x" << memoryEntry->Memory << "\n";
    }

    std::free(memoryEntry->Memory);// NOLINT
    memoryEntryContainer.erase(memoryEntry);
  }

  struct MemoryEntry
  {
    MemoryEntry(void *memory, size_t byteCount, MemoryTag tag) : Memory(memory), ByteCount(byteCount), Tag(tag) {}

    void *Memory;
    size_t ByteCount;
    MemoryTag Tag;
  };
  auto GetMemoryEntryContainer(MemoryTag tag) -> std::vector<MemoryEntry> &
  {
    return m_MemoryEntries.at(static_cast<size_t>(tag));
  }

  std::array<std::vector<MemoryEntry>, static_cast<size_t>(MemoryTag::TAG_COUNT)> m_MemoryEntries;

  static MemoryController *s_MemoryControllerInstance;// NOLINT
};

JE::MemoryController *JE::MemoryController::s_MemoryControllerInstance = nullptr;// NOLINT

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

class Window
{
  friend class WindowController;

public:
  Window(const Window &other) = delete;
  auto operator=(const Window &other) -> Window & = delete;
  Window(Window &&other) = delete;
  auto operator=(Window &&other) -> Window & = delete;

  Window(std::string_view title, const Size2D &size, IPlatformBackend::NativeWindowHandle nativeHandle)
    : m_NativeHandle(nativeHandle), m_Title(title), m_Size(size)
  {}

  ~Window() { IPlatformBackend::Get().DestroyWindow(m_NativeHandle); }

  [[nodiscard]] auto Title() const -> const std::string & { return m_Title; }
  void SetTitle(std::string_view title)
  {
    IPlatformBackend::Get().SetWindowTitle(m_NativeHandle, title);
    m_Title = title;
  }

  [[nodiscard]] auto Size() const -> const Size2D & { return m_Size; }
  void SetSize(const Size2D &size)
  {
    IPlatformBackend::Get().SetWindowSize(m_NativeHandle, size);
    m_Size = size;
  }

private:
  [[nodiscard]] auto GetNativeHandle() const -> IPlatformBackend::NativeWindowHandle { return m_NativeHandle; }

  IPlatformBackend::NativeWindowHandle m_NativeHandle;
  std::string m_Title;
  Size2D m_Size;
};

class WindowController
{
  using WindowContainer = Vector<Scope<Window, MemoryTag::App>, MemoryTag::App>;

public:
  WindowController()
  {
    ASSERT(IPlatformBackend::Get().Initialized(), "Backend needs to be initialized before using this class");
  }

  auto CreateWindow(std::string_view title, const Size2D &size) -> Window &
  {
    m_Windows.push_back(
      CreateScope<Window, MemoryTag::App>(title, size, IPlatformBackend::Get().CreateWindow(title, size)));
    return *m_Windows.back();
  }
  [[nodiscard]] auto Windows() const -> const WindowContainer & { return m_Windows; }

  void DeleteAllWindows() { m_Windows.clear(); }

private:
  WindowContainer m_Windows;
};

}// namespace JE

class WindowControllerTestsFixture
{
public:
  static constexpr auto TEST_WINDOW_TITLE = std::string_view{ "Test Window" };
  static constexpr auto TEST_WINDOW_SIZE = JE::Size2D{ 1280, 720 };

  WindowControllerTestsFixture() { JE::UNUSED(m_Backend.Initialize()); }

protected:
  TestPlatformBackend m_Backend;
  JE::MemoryController m_MemoryController;
  JE::WindowController m_WindowController;
};

TEST_CASE_METHOD(WindowControllerTestsFixture,
  "JE::WindowController creates JE::Window and keeps track of it (Has ownership)",
  "[JE::WindowController]")
{
  auto &window = m_WindowController.CreateWindow(TEST_WINDOW_TITLE, TEST_WINDOW_SIZE);
  auto windowFindFunction = [&](const JE::Scope<JE::Window, JE::MemoryTag::App> &windowEntry) {
    return windowEntry.get() == &window;
  };

  const auto &windows = m_WindowController.Windows();
  auto windowIt = JE::FindIf(windows, windowFindFunction);

  REQUIRE(windowIt != std::end(windows));

  m_WindowController.DeleteAllWindows();
  windowIt = JE::FindIf(windows, windowFindFunction);

  REQUIRE(windowIt == std::end(windows));
}