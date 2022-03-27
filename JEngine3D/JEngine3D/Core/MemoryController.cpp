#include "MemoryController.hpp"

#include <cstdlib>// for free, malloc, size_t
#include <iostream>// for operator<<, basic_ostream, basic_ostream<>::__os...
#include <iterator>// for end

namespace JE {

MemoryController *MemoryController::s_MemoryControllerInstance = nullptr;// NOLINT

MemoryController::MemoryController()
{
  ASSERT(!s_MemoryControllerInstance, "MemoryController instance already created");
  s_MemoryControllerInstance = this;
}
MemoryController::~MemoryController()
{
  for (const auto &memoryTagEntries : m_MemoryEntries) {
    ASSERT(memoryTagEntries.empty(),
      "MemoryController still has allocated entries, there is a memory leak somewhere in the program!");
  }
}

auto MemoryController::Allocate(size_t byteCount, MemoryTag tag) -> void *
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

void MemoryController::Deallocate(void *memory, MemoryTag tag)
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

}// namespace JE