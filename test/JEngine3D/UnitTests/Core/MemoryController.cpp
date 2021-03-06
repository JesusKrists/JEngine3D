#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/MemoryController.hpp>
#include <JEngine3D/Core/Base.hpp>// for FindIf

#include <iterator>// for end
#include <cstddef>// IWYU pragma: keep

class MemoryControllerTestsFixture
{
protected:
    JE::MemoryController m_MemoryController;
};

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE_METHOD(MemoryControllerTestsFixture,
                 "JE::MemoryController allocates and deallocates memory with a tag attached",
                 "[JE::MemoryController]")
{
    REQUIRE(JE::MemoryController::Get().TotalAllocCount() == 0);
    auto* intPtr = JE::MemoryController::Allocate<int, JE::MemoryTag::Test>();
    REQUIRE(JE::MemoryController::Get().TotalAllocCount() != 0);

    REQUIRE(intPtr != nullptr);

    const auto& testMemoryEntries = JE::MemoryController::Get().MemoryEntries().at(static_cast<size_t>(JE::MemoryTag::Test));
    auto entryIt = JE::FindIf(testMemoryEntries, [&](const JE::MemoryController::MemoryEntry& entry) { return entry.Memory == intPtr; });

    REQUIRE(entryIt != std::end(testMemoryEntries));
    REQUIRE(entryIt->Tag == JE::MemoryTag::Test);

    REQUIRE(JE::MemoryController::Get().TotalDeallocCount() == 0);
    JE::MemoryController::Deallocate<int, JE::MemoryTag::Test>(intPtr);
    REQUIRE(JE::MemoryController::Get().TotalDeallocCount() != 0);

    entryIt = JE::FindIf(testMemoryEntries, [&](const JE::MemoryController::MemoryEntry& entry) { return entry.Memory == intPtr; });

    REQUIRE(entryIt == std::end(testMemoryEntries));
}

TEST_CASE_METHOD(MemoryControllerTestsFixture, "JE::CreateScope returns MemoryController Allocated JE::Scope", "[JE::MemoryController]")
{
    constexpr auto intVal   = 10;
    const auto     intScope = JE::CreateScope<int, JE::MemoryTag::Test>(intVal);

    REQUIRE(*intScope == intVal);

    const auto& testMemoryEntries = JE::MemoryController::Get().MemoryEntries().at(static_cast<size_t>(JE::MemoryTag::Test));
    auto        entryIt =
    JE::FindIf(testMemoryEntries, [&](const JE::MemoryController::MemoryEntry& entry) { return entry.Memory == intScope.get(); });

    REQUIRE(entryIt != std::end(testMemoryEntries));
    REQUIRE(entryIt->Tag == JE::MemoryTag::Test);
}

TEST_CASE_METHOD(MemoryControllerTestsFixture, "JE::CreateRef returns MemoryController Allocated JE::Ref", "[JE::MemoryController]")
{
    constexpr auto intVal = 10;
    const auto     intRef = JE::CreateRef<int, JE::MemoryTag::Test>(intVal);

    REQUIRE(*intRef == intVal);

    const auto& testMemoryEntries = JE::MemoryController::Get().MemoryEntries().at(static_cast<size_t>(JE::MemoryTag::Test));
    auto        entryIt =
    JE::FindIf(testMemoryEntries, [&](const JE::MemoryController::MemoryEntry& entry) { return entry.Memory == intRef.get(); });

    REQUIRE(entryIt != std::end(testMemoryEntries));
    REQUIRE(entryIt->Tag == JE::MemoryTag::Test);
}

TEST_CASE_METHOD(MemoryControllerTestsFixture, "JE::Vector allocates from MemoryController with MemoryTag", "[JE::MemoryController]")
{
    JE::Vector<int, JE::MemoryTag::Test> testVec{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };// NOLINT

    const auto& testMemoryEntries = JE::MemoryController::Get().MemoryEntries().at(static_cast<size_t>(JE::MemoryTag::Test));
    auto        entryIt =
    JE::FindIf(testMemoryEntries, [&](const JE::MemoryController::MemoryEntry& entry) { return entry.Memory == testVec.data(); });

    REQUIRE(entryIt != std::end(testMemoryEntries));
    REQUIRE(entryIt->Tag == JE::MemoryTag::Test);
}