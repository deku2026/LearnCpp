// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : no_throw_guarantee
// Topic id : part2/stage09/section03/no_throw_guarantee
//
// Covers: nothrow / noexcept operations that never throw

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <utility>
#include <vector>

namespace {

void swap_ints(int& a, int& b) noexcept {
    int t = a;
    a = b;
    b = t;
}

struct Pod {
    int x = 0;
};

void demo_basics() {
    int a = 1;
    int b = 2;
    swap_ints(a, b);
    LEARN_CHECK(a == 2 && b == 1);
    static_assert(noexcept(swap_ints(a, b)));
}

void demo_intermediate() {
    // vector::swap is noexcept if allocators allow (default is).
    std::vector<int> v1{1, 2};
    std::vector<int> v2{3};
    v1.swap(v2);
    LEARN_CHECK(v1.size() == 1);
    LEARN_CHECK(v2.size() == 2);
    static_assert(noexcept(v1.swap(v2)));
}

void demo_expert() {
    Pod a{1};
    Pod b{2};
    // Trivial types: moves/copies don't throw.
    a = std::move(b);
    LEARN_CHECK(a.x == 2);
    static_assert(std::is_nothrow_move_assignable_v<Pod>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section03/no_throw_guarantee", run>;

}  // namespace
