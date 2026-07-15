// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : implicit_lifetime_types_cpp23
// Topic id : part6/g/section04/implicit_lifetime_types_cpp23
//
// Covers: implicit-lifetime types (C++23 trait when available)

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

void demo_basics() {
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    static_assert(std::is_implicit_lifetime_v<int>);
#else
    static_assert(std::is_trivial_v<int>);
#endif
}

void demo_intermediate() {
    struct Agg {
        int a;
        int b;
    };
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    static_assert(std::is_implicit_lifetime_v<Agg>);
#else
    static_assert(std::is_trivial_v<Agg>);
#endif
    Agg a{1, 2};
    LEARN_CHECK(a.a + a.b == 3);
}

void demo_expert() {
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/implicit_lifetime_types_cpp23", run>;

}  // namespace
