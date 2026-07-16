// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : cache_line_basics
// Topic id : part6/c/section05/cache_line_basics
// Reference: C++23 working draft [support.types.layout].

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <new>

namespace {

#if defined(__cpp_lib_hardware_interference_size) && __cpp_lib_hardware_interference_size >= 201703L
constexpr std::size_t kDestructiveSize = std::hardware_destructive_interference_size;
constexpr std::size_t kConstructiveSize = std::hardware_constructive_interference_size;
#else
constexpr std::size_t kDestructiveSize = 64;
constexpr std::size_t kConstructiveSize = 64;
#endif

struct alignas(kDestructiveSize) IsolatedCounter {
    std::atomic<int> value{};
};

struct FrequentlyReadTogether {
    int size;
    int capacity;
    const void* data;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section05/cache_line_basics";
    learn::ExampleChecks checks{topic};

    static_assert(kDestructiveSize > 0);
    static_assert(kConstructiveSize > 0);
    static_assert(alignof(IsolatedCounter) == kDestructiveSize);
    std::array<IsolatedCounter, 2> counters{};
    const auto first = reinterpret_cast<std::uintptr_t>(&counters[0]);
    const auto second = reinterpret_cast<std::uintptr_t>(&counters[1]);
    LEARN_EXPECT(checks, second - first >= kDestructiveSize);
    LEARN_EXPECT_EQ(checks, first % alignof(IsolatedCounter), std::uintptr_t{0});

    static_assert(sizeof(FrequentlyReadTogether) <= kConstructiveSize);
    FrequentlyReadTogether metadata{3, 8, counters.data()};
    LEARN_EXPECT_EQ(checks, metadata.size, 3);
    LEARN_EXPECT_EQ(checks, metadata.capacity, 8);

    // The interference constants are portable hints, not a runtime cache-discovery API. Alignment
    // can reduce accidental sharing, but layout, access pattern, NUMA placement and measurement on
    // the target hardware still determine performance.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/cache_line_basics", run>;

}  // namespace
