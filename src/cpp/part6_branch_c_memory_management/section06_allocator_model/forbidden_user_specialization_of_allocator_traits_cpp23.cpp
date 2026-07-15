// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : forbidden_user_specialization_of_allocator_traits_cpp23
// Topic id : part6/c/section06/forbidden_user_specialization_of_allocator_traits_cpp23
//
// Covers: do not specialize allocator_traits; customize the allocator type

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>

namespace {

// C++23 clarifies users must not specialize std::allocator_traits.
// Provide behavior on the allocator itself instead.

template <class T>
struct MyAlloc {
    using value_type = T;
    using is_always_equal = std::true_type;
    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }
};

void demo_basics() {
    MyAlloc<int> a;
    int* p = a.allocate(1);
    *p = 4;
    assert(*p == 4);
    a.deallocate(p, 1);
}

void demo_intermediate() {
    using AT = std::allocator_traits<MyAlloc<int>>;
    static_assert(AT::is_always_equal::value);
}

void demo_expert() {
    // Specialize allocator, not allocator_traits.
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/c/section06/forbidden_user_specialization_of_allocator_traits_cpp23", run>;

}  // namespace
