// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : false_sharing
// Topic id : part6/c/section05/false_sharing
// Reference: C++23 working draft [support.types.layout], [atomics.order].

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <new>
#include <thread>

namespace {

#if defined(__cpp_lib_hardware_interference_size) && __cpp_lib_hardware_interference_size >= 201703L
constexpr std::size_t kCacheSeparation = std::hardware_destructive_interference_size;
#else
constexpr std::size_t kCacheSeparation = 64;
#endif

struct PackedCounters {
    std::atomic<std::uint64_t> first{};
    std::atomic<std::uint64_t> second{};
};

struct alignas(kCacheSeparation) PaddedCounter {
    std::atomic<std::uint64_t> value{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section05/false_sharing";
    learn::ExampleChecks checks{topic};
    PackedCounters packed{};
    LEARN_EXPECT(checks, &packed.first != &packed.second);

    std::array<PaddedCounter, 2> separated{};
    const auto first_address = reinterpret_cast<std::uintptr_t>(&separated[0]);
    const auto second_address = reinterpret_cast<std::uintptr_t>(&separated[1]);
    LEARN_EXPECT(checks, second_address - first_address >= kCacheSeparation);

    constexpr std::uint64_t iterations = 10'000;
    std::thread first_worker{[&] {
        for (std::uint64_t index = 0; index < iterations; ++index) {
            separated[0].value.fetch_add(1, std::memory_order_relaxed);
        }
    }};
    std::thread second_worker{[&] {
        for (std::uint64_t index = 0; index < iterations; ++index) {
            separated[1].value.fetch_add(1, std::memory_order_relaxed);
        }
    }};
    first_worker.join();
    second_worker.join();
    LEARN_EXPECT_EQ(checks, separated[0].value.load(std::memory_order_relaxed), iterations);
    LEARN_EXPECT_EQ(checks, separated[1].value.load(std::memory_order_relaxed), iterations);

    // Packed atomics are race-free yet may still bounce one cache line between cores. Padding is a
    // layout optimization, not a synchronization mechanism; relaxed ordering is sufficient here
    // only because each final value is read after thread joins.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/false_sharing", run>;

}  // namespace
