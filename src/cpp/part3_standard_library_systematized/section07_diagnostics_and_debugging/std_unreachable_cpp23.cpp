// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : std_unreachable_cpp23
// Topic id : part3/section07/std_unreachable_cpp23
//
// Covers: std::unreachable marks impossible paths (do not call)

#include "learn/topic_registry.hpp"

#include <utility>
#include <version>

namespace {

void demo_basics() {
    // Never call std::unreachable() in reachable code — it is UB by definition.
    int x = 1;
    if (x == 1) {
        LEARN_CHECK(true);
    } else {
        // std::unreachable();  // only if x cannot be anything else
    }
}

void demo_intermediate() {
    enum class Color { Red, Blue };
    Color c = Color::Red;
    int v = 0;
    switch (c) {
        case Color::Red:
            v = 1;
            break;
        case Color::Blue:
            v = 2;
            break;
    }
    LEARN_CHECK(v == 1);
}

void demo_expert() {
#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
    // Available as std::unreachable(); still must be unreachable
    LEARN_CHECK(__cpp_lib_unreachable >= 202202L);
#else
    LEARN_CHECK(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section07/std_unreachable_cpp23", run>;

}  // namespace
