// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : is_implicit_lifetime_in_templates_cpp23
// Topic id : part6/e/section10/is_implicit_lifetime_in_templates_cpp23
//
// Covers: is_implicit_lifetime used in template constraints

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

template <class T>
constexpr bool can_start_in_bytes() {
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    return std::is_implicit_lifetime_v<T>;
#else
    return std::is_trivial_v<T>;
#endif
}

void demo_basics() {
    static_assert(can_start_in_bytes<int>());
}

void demo_intermediate() {
    struct Agg {
        int a;
        int b;
    };
    LEARN_CHECK(can_start_in_bytes<Agg>());
}

void demo_expert() {
    // Non-trivial dtor ⇒ not implicit-lifetime (user ctor alone is insufficient).
    struct Non {
        int x = 0;
        ~Non() { x = -1; }
    };
    LEARN_CHECK(!can_start_in_bytes<Non>());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section10/is_implicit_lifetime_in_templates_cpp23", run>;

}  // namespace
