// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section04_false_sharing_and_barriers
// Item     : false_sharing_revisited
// Topic id : part6/f/section04/false_sharing_revisited
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
constexpr std::size_t kSeparation = std::hardware_destructive_interference_size;
#else
constexpr std::size_t kSeparation = 64;
#endif

struct PackedPair {
    std::atomic<int> first{};
    std::atomic<int> second{};
};

struct alignas(kSeparation) IsolatedAtomic {
    std::atomic<int> value{};
};

template <class First, class Second>
void increment_pair(First& first, Second& second) {
    constexpr int iterations = 2'000;
    std::thread left{[&] {
        for (int index = 0; index < iterations; ++index) {
            first.fetch_add(1, std::memory_order_relaxed);
        }
    }};
    std::thread right{[&] {
        for (int index = 0; index < iterations; ++index) {
            second.fetch_add(1, std::memory_order_relaxed);
        }
    }};
    left.join();
    right.join();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/f/section04/false_sharing_revisited";
    learn::ExampleChecks checks{topic};

    PackedPair packed;
    increment_pair(packed.first, packed.second);
    LEARN_EXPECT_EQ(checks, packed.first.load(std::memory_order_relaxed), 2'000);
    LEARN_EXPECT_EQ(checks, packed.second.load(std::memory_order_relaxed), 2'000);

    std::array<IsolatedAtomic, 2> isolated{};
    increment_pair(isolated[0].value, isolated[1].value);
    LEARN_EXPECT_EQ(checks, isolated[0].value.load(std::memory_order_relaxed), 2'000);
    LEARN_EXPECT_EQ(checks, isolated[1].value.load(std::memory_order_relaxed), 2'000);
    const auto first = reinterpret_cast<std::uintptr_t>(&isolated[0]);
    const auto second = reinterpret_cast<std::uintptr_t>(&isolated[1]);
    LEARN_EXPECT(checks, second - first >= kSeparation);

    // Both layouts are race-free and produce the same result. The packed pair may share a cache
    // line and incur coherence traffic, but that is a hardware/layout fact to profile—not a stable
    // timing assertion. Padding increases footprint and can hurt constructive locality.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section04/false_sharing_revisited", run>;

}  // namespace
