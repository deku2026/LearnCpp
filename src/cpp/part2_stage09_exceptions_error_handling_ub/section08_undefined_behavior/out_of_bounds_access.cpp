// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : out_of_bounds_access
// Topic id : part2/stage09/section08/out_of_bounds_access
//
// Covers: OOB is UB — use size checks / at()

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <stdexcept>
#include <vector>

namespace {

int get_checked(const std::vector<int>& v, std::size_t i) {
    if (i >= v.size()) {
        return -1;
    }
    return v[i];
}

void demo_basics() {
    std::vector<int> v{10, 20, 30};
    assert(get_checked(v, 1) == 20);
    assert(get_checked(v, 99) == -1);
}

void demo_intermediate() {
    std::vector<int> v{1, 2};
    try {
        (void)v.at(5);  // throws, defined
        assert(false);
    } catch (const std::out_of_range&) {
    }
    assert(v.at(0) == 1);
}

void demo_expert() {
    std::array<int, 3> a{1, 2, 3};
    assert(a.size() == 3);
    // SAFE: iterate with range-for, never a[3].
    int s = 0;
    for (int x : a) {
        s += x;
    }
    assert(s == 6);
    // Comment only: `a[3]` is UB.
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/out_of_bounds_access", run>;

}  // namespace
