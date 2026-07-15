// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : if_consteval_branch_cpp23
// Topic id : part2/stage15/section03/if_consteval_branch_cpp23
//
// Covers: if consteval branch (C++23)

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <utility>

namespace {

constexpr int mode() {
#if defined(__cpp_if_consteval)
    if consteval {
        return 1;
    } else {
        return 2;
    }
#else
    if (std::is_constant_evaluated()) {
        return 1;
    }
    return 2;
#endif
}

void demo_basics() {
    LEARN_CHECK(mode() == 2);
}

void demo_intermediate() {
    constexpr int c = mode();
    LEARN_CHECK(c == 1 || c == 2);
    // At runtime mode() is 2; as constant expression it is 1 when supported.
#if defined(__cpp_if_consteval) || defined(__cpp_lib_is_constant_evaluated)
    static_assert(mode() == 1);
#endif
}

void demo_expert() {
    auto runtime = mode();
    LEARN_CHECK(runtime == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/if_consteval_branch_cpp23", run>;

}  // namespace
