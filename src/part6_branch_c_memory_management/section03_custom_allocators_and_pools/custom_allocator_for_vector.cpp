// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section03_custom_allocators_and_pools
// Item     : custom_allocator_for_vector
// Topic id : part6/c/section03/custom_allocator_for_vector
// Reference: C++23 working draft [allocator.requirements], [vector].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <vector>

namespace {

struct AllocationStats {
    std::size_t allocated_bytes{};
    std::size_t deallocated_bytes{};
    int allocation_calls{};
    int deallocation_calls{};
};

template <class T>
struct CountingAllocator {
    using value_type = T;

    AllocationStats* stats{};

    explicit CountingAllocator(AllocationStats& observed) noexcept : stats(&observed) {}

    template <class U>
    CountingAllocator(const CountingAllocator<U>& other) noexcept : stats(other.stats) {}

    [[nodiscard]] T* allocate(std::size_t count) {
        stats->allocated_bytes += count * sizeof(T);
        ++stats->allocation_calls;
        return std::allocator<T>{}.allocate(count);
    }

    void deallocate(T* pointer, std::size_t count) noexcept {
        stats->deallocated_bytes += count * sizeof(T);
        ++stats->deallocation_calls;
        std::allocator<T>{}.deallocate(pointer, count);
    }

    template <class U>
    friend struct CountingAllocator;

    template <class U>
    friend bool operator==(const CountingAllocator& left, const CountingAllocator<U>& right) noexcept {
        return left.stats == right.stats;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section03/custom_allocator_for_vector";
    learn::ExampleChecks checks{topic};
    AllocationStats stats{};

    {
        std::vector<int, CountingAllocator<int>> values{CountingAllocator<int>{stats}};
        values.reserve(8);
        for (int value = 1; value <= 6; ++value) {
            values.push_back(value * value);
        }
        LEARN_EXPECT_EQ(checks, values.size(), std::size_t{6});
        LEARN_EXPECT_EQ(checks, values.back(), 36);
        LEARN_EXPECT(checks, stats.allocation_calls >= 1);
        LEARN_EXPECT(checks, stats.allocated_bytes >= values.capacity() * sizeof(int));
    }

    LEARN_EXPECT_EQ(checks, stats.allocation_calls, stats.deallocation_calls);
    LEARN_EXPECT_EQ(checks, stats.allocated_bytes, stats.deallocated_bytes);
    // vector controls element construction, growth and rollback through allocator_traits. The
    // allocator owns only raw storage; it must not assume that every allocated slot holds an object.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section03/custom_allocator_for_vector", run>;

}  // namespace
