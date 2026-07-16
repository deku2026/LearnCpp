// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : monotonic_buffer_resource
// Topic id : part6/c/section06/monotonic_buffer_resource
// Reference: C++23 working draft [mem.res.monotonic.buffer].

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <memory_resource>
#include <numeric>
#include <string>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/monotonic_buffer_resource";
    learn::ExampleChecks checks{topic};
    alignas(std::max_align_t) std::array<std::byte, 2048> buffer{};
    std::pmr::monotonic_buffer_resource arena{buffer.data(), buffer.size(), std::pmr::null_memory_resource()};

    {
        std::pmr::vector<int> values{&arena};
        values.resize(32);
        std::iota(values.begin(), values.end(), 1);
        std::pmr::string label{"request-local data", &arena};
        LEARN_EXPECT_EQ(checks, values.front(), 1);
        LEARN_EXPECT_EQ(checks, values.back(), 32);
        LEARN_EXPECT_EQ(checks, label, (std::pmr::string{"request-local data", &arena}));
    }

    // Container deallocate calls are intentionally ignored. release reclaims all arena allocations
    // together and is safe only after every object and pointer into the arena is no longer used.
    arena.release();
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/monotonic_buffer_resource", run>;

}  // namespace
