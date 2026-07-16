// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : allocate_at_least_cpp23
// Topic id : part3/section06/allocate_at_least_cpp23
// References: C++23 [allocator.traits.members], P0401R6

#include "learn/example_support.hpp"

#include <cstddef>
#include <limits>
#include <memory>
#include <new>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section06/allocate_at_least_cpp23";

#if defined(__cpp_lib_allocate_at_least) && __cpp_lib_allocate_at_least >= 202302L
template <class T>
class GrowthAllocator {
public:
    using value_type = T;

    GrowthAllocator() = default;

    template <class U>
    GrowthAllocator(const GrowthAllocator<U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t count) { return std::allocator<T>{}.allocate(count); }

    [[nodiscard]] std::allocation_result<T*> allocate_at_least(std::size_t requested) {
        constexpr std::size_t extra = 3;
        if (requested > std::numeric_limits<std::size_t>::max() - extra) {
            throw std::bad_array_new_length{};
        }
        const std::size_t actual = requested + extra;
        return {allocate(actual), actual};
    }

    void deallocate(T* pointer, std::size_t count) noexcept { std::allocator<T>{}.deallocate(pointer, count); }

    template <class U>
    friend bool operator==(const GrowthAllocator&, const GrowthAllocator<U>&) noexcept {
        return true;
    }
};
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_allocate_at_least) && __cpp_lib_allocate_at_least >= 202302L
    ::learn::ExampleChecks checks{kTopic};
    GrowthAllocator<int> allocator;
    using Traits = std::allocator_traits<GrowthAllocator<int>>;
    constexpr std::size_t requested = 4;
    auto allocation = Traits::allocate_at_least(allocator, requested);
    LEARN_EXPECT(checks, allocation.ptr != nullptr);
    LEARN_EXPECT_EQ(checks, allocation.count, std::size_t{7});

    std::size_t constructed{};
    try {
        for (; constructed < requested; ++constructed) {
            Traits::construct(allocator, allocation.ptr + constructed, static_cast<int>((constructed + 1) * 10));
        }
        LEARN_EXPECT_EQ(checks, allocation.ptr[0], 10);
        LEARN_EXPECT_EQ(checks, allocation.ptr[3], 40);
    } catch (...) {
        while (constructed != 0) {
            Traits::destroy(allocator, allocation.ptr + --constructed);
        }
        Traits::deallocate(allocator, allocation.ptr, allocation.count);
        throw;
    }
    while (constructed != 0) {
        Traits::destroy(allocator, allocation.ptr + --constructed);
    }
    // Deallocate with the actual count returned by allocate_at_least.
    Traits::deallocate(allocator, allocation.ptr, allocation.count);

    std::allocator<int> standard_allocator;
    auto standard = std::allocator_traits<std::allocator<int>>::allocate_at_least(standard_allocator, 5);
    LEARN_EXPECT(checks, standard.count >= 5);
    std::allocator_traits<std::allocator<int>>::deallocate(standard_allocator, standard.ptr, standard.count);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "allocator_traits::allocate_at_least");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/allocate_at_least_cpp23", run>;

}  // namespace
