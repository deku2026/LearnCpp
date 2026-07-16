// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : allocator_requirements
// Topic id : part6/c/section06/allocator_requirements
// Reference: C++23 working draft [allocator.requirements].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>

namespace {

template <class T>
struct MinimalAllocator {
    using value_type = T;

    MinimalAllocator() noexcept = default;

    template <class U>
    MinimalAllocator(const MinimalAllocator<U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t count) { return std::allocator<T>{}.allocate(count); }

    void deallocate(T* pointer, std::size_t count) noexcept { std::allocator<T>{}.deallocate(pointer, count); }
};

template <class T, class U>
bool operator==(const MinimalAllocator<T>&, const MinimalAllocator<U>&) noexcept {
    return true;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/allocator_requirements";
    learn::ExampleChecks checks{topic};
    using Allocator = MinimalAllocator<int>;
    using Traits = std::allocator_traits<Allocator>;

    static_assert(std::is_same_v<Allocator::value_type, int>);
    static_assert(std::is_nothrow_copy_constructible_v<Allocator>);
    static_assert(Traits::is_always_equal::value);

    Allocator allocator;
    int* storage = Traits::allocate(allocator, 3);
    for (int index = 0; index < 3; ++index) {
        Traits::construct(allocator, storage + index, index + 1);
    }
    LEARN_EXPECT_EQ(checks, storage[0] + storage[1] + storage[2], 6);
    for (int index = 3; index != 0; --index) {
        Traits::destroy(allocator, storage + index - 1);
    }
    Traits::deallocate(allocator, storage, 3);

    std::vector<int, Allocator> values{2, 3, 5, 7};
    LEARN_EXPECT_EQ(checks, values.size(), std::size_t{4});
    LEARN_EXPECT(checks, values.get_allocator() == allocator);
    // allocate returns raw uninitialized storage; n passed to deallocate must match the allocation.
    // Containers add exception-safe element construction and destruction through allocator_traits.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/allocator_requirements", run>;

}  // namespace
