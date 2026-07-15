// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : assume_unreachable_misuse_cpp23
// Topic id : part6/g/section03/assume_unreachable_misuse_cpp23
//
// Covers: [[assume]] / unreachable misuse can invent UB; use carefully

#include "learn/topic_registry.hpp"

#include <cassert>
#include <utility>

namespace {

int abs_positive(int x) {
    if (x < 0) return -x;
    return x;
}

void demo_basics() {
    assert(abs_positive(-3) == 3);
    assert(abs_positive(2) == 2);
}

void demo_intermediate() {
#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
    auto classify = [](int x) -> int {
        if (x == 0) return 0;
        if (x > 0) return 1;
        if (x < 0) return -1;
        std::unreachable();  // only if all cases covered
    };
    assert(classify(0) == 0);
    assert(classify(5) == 1);
    assert(classify(-2) == -1);
#else
    assert(true);
#endif
}

void demo_expert() {
    // Teaching: lying to the compiler with assume/unreachable about reachable states is UB.
    // Prefer real checks in general code.
    assert(abs_positive(0) == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/assume_unreachable_misuse_cpp23", run>;

}  // namespace
