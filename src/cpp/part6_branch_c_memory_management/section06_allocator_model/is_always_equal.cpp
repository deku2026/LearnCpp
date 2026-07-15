// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section06_allocator_model
// Item     : is_always_equal
// Topic id : part6/c/section06/is_always_equal
//
// Covers: allocator_traits::is_always_equal

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <type_traits>

namespace {

void demo_basics() {
    using AT = std::allocator_traits<std::allocator<int>>;
    static_assert(AT::is_always_equal::value);
}

void demo_intermediate() {
    std::allocator<int> a, b;
    assert(a == b);
}

void demo_expert() {
    // Stateful allocators may set is_always_equal = false_type.
    struct Stateful {
        using value_type = int;
        int id = 0;
        int* allocate(std::size_t n) { return static_cast<int*>(::operator new(n * sizeof(int))); }
        void deallocate(int* p, std::size_t) noexcept { ::operator delete(p); }
        using is_always_equal = std::false_type;
        bool operator==(const Stateful& o) const { return id == o.id; }
    };
    static_assert(!std::allocator_traits<Stateful>::is_always_equal::value);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section06/is_always_equal", run>;

}  // namespace
