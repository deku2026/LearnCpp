// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : forbidden_user_specialization_of_allocator_traits_cpp23
// Topic id : part6/c/section06/forbidden_user_specialization_of_allocator_traits_cpp23
// Reference: C++23 working draft [allocator.traits]: user specializations are ill-formed.

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>

namespace branch_c_allocator_policy {

template <class T>
struct PolicyAllocator {
    using value_type = T;
    using size_type = std::size_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    PolicyAllocator() = default;

    template <class U>
    PolicyAllocator(const PolicyAllocator<U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t count) { return std::allocator<T>{}.allocate(count); }
    void deallocate(T* pointer, std::size_t count) noexcept { std::allocator<T>{}.deallocate(pointer, count); }
};

template <class T, class U>
bool operator==(const PolicyAllocator<T>&, const PolicyAllocator<U>&) noexcept {
    return true;
}

}  // namespace branch_c_allocator_policy

#if 0
// Ill-formed in C++23 even if an implementation appears to accept it:
template <>
struct std::allocator_traits<branch_c_allocator_policy::PolicyAllocator<int>> {};
#endif

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section06/forbidden_user_specialization_of_allocator_traits_cpp23";
    learn::ExampleChecks checks{topic};
    using Allocator = branch_c_allocator_policy::PolicyAllocator<int>;
    using Traits = std::allocator_traits<Allocator>;

    // Customize through members on the allocator itself; allocator_traits detects them.
    static_assert(std::is_same_v<Traits::size_type, std::size_t>);
    static_assert(Traits::propagate_on_container_move_assignment::value);
    static_assert(Traits::is_always_equal::value);
    static_assert(std::is_same_v<Traits::rebind_alloc<long>, branch_c_allocator_policy::PolicyAllocator<long>>);

    std::vector<int, Allocator> values{2, 3, 5};
    LEARN_EXPECT_EQ(checks, values.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, values.back(), 5);
    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/c/section06/forbidden_user_specialization_of_allocator_traits_cpp23", run>;

}  // namespace
