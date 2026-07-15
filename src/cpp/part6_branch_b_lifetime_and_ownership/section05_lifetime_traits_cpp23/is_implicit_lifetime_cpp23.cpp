// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : is_implicit_lifetime_cpp23
// Topic id : part6/b/section05/is_implicit_lifetime_cpp23
//
// Covers: std::is_implicit_lifetime (C++23) feature-test gated

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

void demo_basics() {
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    static_assert(std::is_implicit_lifetime_v<int>);
    struct Agg {
        int x;
        int y;
    };
    static_assert(std::is_implicit_lifetime_v<Agg>);
#else
    // Implicit-lifetime types include scalars and aggregates of such — conceptual check.
    static_assert(std::is_trivial_v<int>);
    struct Agg {
        int x;
        int y;
    };
    static_assert(std::is_trivial_v<Agg>);
#endif
}

void demo_intermediate() {
    // Non-trivial dtor → not implicit-lifetime (trivial copy/move alone is not enough).
    struct WithUserDtor {
        int x = 0;
        ~WithUserDtor() { x = -1; }
    };
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    static_assert(!std::is_implicit_lifetime_v<WithUserDtor>);
#else
    static_assert(!std::is_trivially_destructible_v<WithUserDtor>);
#endif
    assert(true);
}

void demo_expert() {
    // Implicit lifetime matters for start_lifetime_as / byte buffers in systems code.
    alignas(int) unsigned char buf[sizeof(int)]{};
    assert(sizeof(buf) == sizeof(int));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section05/is_implicit_lifetime_cpp23", run>;

}  // namespace
