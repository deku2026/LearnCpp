// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : propagate_on_container_copy_move_swap
// Topic id : part6/c/section06/propagate_on_container_copy_move_swap
// Reference: C++23 working draft [allocator.traits], [container.alloc.reqmts].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>

namespace {

template <class T>
struct PropagatingAllocator {
    using value_type = T;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    int id{};

    explicit PropagatingAllocator(int value) noexcept : id(value) {}
    template <class U>
    PropagatingAllocator(const PropagatingAllocator<U>& other) noexcept : id(other.id) {}

    [[nodiscard]] T* allocate(std::size_t count) { return std::allocator<T>{}.allocate(count); }
    void deallocate(T* pointer, std::size_t count) noexcept { std::allocator<T>{}.deallocate(pointer, count); }

    template <class U>
    friend struct PropagatingAllocator;

    template <class U>
    friend bool operator==(const PropagatingAllocator& left, const PropagatingAllocator<U>& right) noexcept {
        return left.id == right.id;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/propagate_on_container_copy_move_swap";
    learn::ExampleChecks checks{topic};
    using Allocator = PropagatingAllocator<int>;
    using Vector = std::vector<int, Allocator>;
    using Traits = std::allocator_traits<Allocator>;
    static_assert(Traits::propagate_on_container_copy_assignment::value);
    static_assert(Traits::propagate_on_container_move_assignment::value);
    static_assert(Traits::propagate_on_container_swap::value);

    Vector source({1, 2}, Allocator{1});
    Vector copy_target({9}, Allocator{2});
    copy_target = source;
    LEARN_EXPECT_EQ(checks, copy_target.get_allocator().id, 1);

    Vector move_target({8}, Allocator{3});
    move_target = std::move(copy_target);
    LEARN_EXPECT_EQ(checks, move_target.get_allocator().id, 1);
    LEARN_EXPECT_EQ(checks, move_target.size(), std::size_t{2});
    LEARN_EXPECT_EQ(checks, move_target.front(), 1);
    LEARN_EXPECT_EQ(checks, move_target.back(), 2);

    Vector left({4}, Allocator{4});
    Vector right({5}, Allocator{5});
    left.swap(right);
    LEARN_EXPECT_EQ(checks, left.get_allocator().id, 5);
    LEARN_EXPECT_EQ(checks, right.get_allocator().id, 4);
    LEARN_EXPECT_EQ(checks, left.front(), 5);
    LEARN_EXPECT_EQ(checks, right.front(), 4);

    // These traits are type-level policy: set them true only when allocator assignment/swap is
    // noexcept as required and transferring resource identity is semantically correct.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/propagate_on_container_copy_move_swap", run>;

}  // namespace
