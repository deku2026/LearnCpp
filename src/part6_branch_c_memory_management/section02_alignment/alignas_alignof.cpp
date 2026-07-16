// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section02_alignment
// Item     : alignas_alignof
// Topic id : part6/c/section02/alignas_alignof
// Reference: C++23 working draft [dcl.align], [expr.alignof], [ptr.align].

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

namespace {

struct alignas(32) AlignedBlock {
    std::array<std::byte, 32> bytes{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section02/alignas_alignof";
    learn::ExampleChecks checks{topic};

    static_assert(alignof(AlignedBlock) == 32);
    static_assert(sizeof(AlignedBlock) % alignof(AlignedBlock) == 0);
    std::array<AlignedBlock, 3> blocks{};
    for (const auto& block : blocks) {
        const auto address = reinterpret_cast<std::uintptr_t>(&block);
        LEARN_EXPECT_EQ(checks, address % alignof(AlignedBlock), std::uintptr_t{0});
    }

    // std::align advances a candidate pointer and reduces the remaining-space count. It neither
    // allocates storage nor starts an object's lifetime.
    std::array<std::byte, 128> raw{};
    void* candidate = raw.data() + 1;
    std::size_t remaining = raw.size() - 1;
    void* aligned = std::align(alignof(AlignedBlock), sizeof(AlignedBlock), candidate, remaining);
    LEARN_EXPECT(checks, aligned != nullptr);
    if (aligned != nullptr) {
        LEARN_EXPECT_EQ(checks, reinterpret_cast<std::uintptr_t>(aligned) % alignof(AlignedBlock), std::uintptr_t{0});
    }

    static_assert(alignof(AlignedBlock) >= alignof(std::max_align_t));
    // alignas may strengthen alignment but cannot request a weaker alignment than the type needs;
    // requested alignments must be valid implementation-supported powers of two.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section02/alignas_alignof", run>;

}  // namespace
