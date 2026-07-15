// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section09_constexpr_and_tmp_fusion
// Item     : if_consteval_in_metaprogramming_cpp23
// Topic id : part6/e/section09/if_consteval_in_metaprogramming_cpp23
//
// Covers: if consteval for compile-time vs runtime paths (C++23)

#include "learn/topic_registry.hpp"

namespace {

constexpr int answer() {
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    if consteval {
        return 42;
    } else {
        return 24;
    }
#else
    return 42;
#endif
}

void demo_basics() {
    constexpr int c = answer();
    LEARN_CHECK(c == 42 || c == 24);
}

void demo_intermediate() {
    int r = answer();
    LEARN_CHECK(r == 42 || r == 24);
}

void demo_expert() {
    static_assert(answer() == 42 || answer() == 24);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section09/if_consteval_in_metaprogramming_cpp23", run>;

}  // namespace
