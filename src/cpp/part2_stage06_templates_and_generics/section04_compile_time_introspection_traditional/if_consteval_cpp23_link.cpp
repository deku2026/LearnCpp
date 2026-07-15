// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : if_consteval_cpp23_link
// Topic id : part2/stage06/section04/if_consteval_cpp23_link
//
// Covers: link to if consteval (C++23) vs if constexpr; portable fallback

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>
#include <version>

namespace {

constexpr int mode_marker() {
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    if consteval {
        return 1;  // constant evaluation context
    } else {
        return 2;  // runtime context
    }
#else
    if (std::is_constant_evaluated()) {
        return 1;
    }
    return 2;
#endif
}

void demo_basics() {
    constexpr int c = mode_marker();
    static_assert(c == 1);
    assert(c == 1);
}

void demo_intermediate() {
    int r = mode_marker();
    assert(r == 2);
}

void demo_expert() {
    // if constexpr picks on type/value known at compile time;
    // if consteval picks on evaluation context (compile vs run).
    auto pick = [](auto x) {
        if constexpr (std::is_integral_v<decltype(x)>) {
            return mode_marker() + 10;
        } else {
            return mode_marker() + 20;
        }
    };
    constexpr int a = pick(1);
    static_assert(a == 11);
    assert(pick(1.0) == 22);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/if_consteval_cpp23_link", run>;

}  // namespace
