#include "MemoryController.hpp"

#include <cstdlib>// for free, malloc, size_t
#include <iterator>// for end
#include <exception>// for exception

namespace JE {

MemoryController *MemoryController::s_MemoryControllerInstance = nullptr;// NOLINT

MemoryController::MemoryController()
{
  if (s_MemoryControllerInstance != nullptr) {
    spdlog::error("MemoryController instance already created");
    DEBUGBREAK();
  }
  s_MemoryControllerInstance = this;
}
MemoryController::~MemoryController()
{
  for (const auto &memoryTagEntries : m_MemoryEntries) {
    if (!memoryTagEntries.empty()) {
      spdlog::error("MemoryController still has allocated entries, there is a memory leak somewhere in the program!");
      DEBUGBREAK();
    }
  }
  s_MemoryControllerInstance = nullptr;
}

auto MemoryController::Allocate(size_t byteCount, MemoryTag tag) -> void *
{
  auto *memory = std::malloc(byteCount);// NOLINT
  if (memory == nullptr) {
    spdlog::error("Failed to allocate memory from the heap");
    DEBUGBREAK();
  }
  GetMemoryEntryContainer(tag).emplace_back(memory, byteCount, tag);

  if (tag == MemoryTag::Unknown) {
    spdlog::warn("Allocating unknown memory tag, reclass this allocation! - {} bytes @ 0x{}", byteCount, memory);
  }

  return memory;
}

void MemoryController::Deallocate(void *memory, MemoryTag tag)
{
  auto &memoryEntryContainer = GetMemoryEntryContainer(tag);

  auto memoryEntry =
    FindIf(memoryEntryContainer, [&memory](const MemoryEntry &entry) { return entry.Memory == memory; });

  if (memoryEntry == std::end(memoryEntryContainer)) {
    spdlog::error("Memory entry does not exist for this allocation, invalid deallocation");
    DEBUGBREAK();
  }

  if (memoryEntry->Tag != tag) {
    spdlog::critical("Deallocate: MemoryTag mismatch for memory 0x{}", memoryEntry->Memory);
  }

  std::free(memoryEntry->Memory);// NOLINT
  memoryEntryContainer.erase(memoryEntry);
}

}// namespace JE