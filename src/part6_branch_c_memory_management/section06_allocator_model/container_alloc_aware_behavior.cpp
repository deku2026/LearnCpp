// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : container_alloc_aware_behavior
// Topic id : part6/c/section06/container_alloc_aware_behavior
// Reference: C++23 working draft [container.alloc.reqmts], [allocator.traits].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>

namespace {

template <class T>
struct StickyAllocator {
    using value_type = T;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;
    using is_always_equal = std::false_type;

    int id{};

    explicit StickyAllocator(int value) noexcept : id(value) {}

    template <class U>
    StickyAllocator(const StickyAllocator<U>& other) noexcept : id(other.id) {}

    [[nodiscard]] T* allocate(std::size_t count) { return std::allocator<T>{}.allocate(count); }
    void deallocate(T* pointer, std::size_t count) noexcept { std::allocator<T>{}.deallocate(pointer, count); }

    [[nodiscard]] StickyAllocator select_on_container_copy_construction() const noexcept {
        return StickyAllocator{id + 100};
    }

    template <class U>
    friend struct StickyAllocator;

    template <class U>
    friend bool operator==(const StickyAllocator& left, const StickyAllocator<U>& right) noexcept {
        return left.id == right.id;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/container_alloc_aware_behavior";
    learn::ExampleChecks checks{topic};
    using Vector = std::vector<int, StickyAllocator<int>>;

    Vector source({1, 2, 3}, StickyAllocator<int>{1});
    Vector copy{source};
    LEARN_EXPECT_EQ(checks, copy.get_allocator().id, 101);
    LEARN_EXPECT_EQ(checks, copy, source);

    Vector copy_target({9}, StickyAllocator<int>{2});
    copy_target = source;
    LEARN_EXPECT_EQ(checks, copy_target.get_allocator().id, 2);
    LEARN_EXPECT_EQ(checks, copy_target, source);

    Vector move_target({8}, StickyAllocator<int>{3});
    move_target = std::move(source);
    LEARN_EXPECT_EQ(checks, move_target.get_allocator().id, 3);
    LEARN_EXPECT_EQ(checks, move_target.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, move_target.front(), 1);
    LEARN_EXPECT_EQ(checks, move_target.back(), 3);

    // Copy construction uses select_on_container_copy_construction. Assignment follows propagation
    // traits; unequal non-propagating move assignment cannot simply steal storage from its source.
#if 0
    Vector left({1}, StickyAllocator<int>{4});
    Vector right({2}, StickyAllocator<int>{5});
    left.swap(right);  // Undefined behavior: unequal allocators and swap does not propagate.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/container_alloc_aware_behavior", run>;

}  // namespace
