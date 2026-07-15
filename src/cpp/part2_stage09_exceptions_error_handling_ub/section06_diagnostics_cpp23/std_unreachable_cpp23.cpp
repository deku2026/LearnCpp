// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : std_unreachable_cpp23
// Topic id : part2/stage09/section06/std_unreachable_cpp23
//
// Covers: std::unreachable (C++23) — teach safely, never call it

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <version>

#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
#include <utility>
#endif

namespace {

const char* color_name(int c) {
    switch (c) {
        case 0:
            return "red";
        case 1:
            return "green";
        case 2:
            return "blue";
        default:
            // SAFE: do not call std::unreachable() here with a reachable default.
            // Document: if the default is truly impossible, unreachable may be used;
            // calling it when reachable is UB.
            return "unknown";
    }
}

int only_zero_one(int x) {
    if (x == 0) {
        return 10;
    }
    if (x == 1) {
        return 20;
    }
    // Exhaustive for our callers; return defensive value instead of unreachable.
    return -1;
}

void demo_basics() {
    assert(std::string{color_name(0)} == "red");
    assert(std::string{color_name(1)} == "green");
}

void demo_intermediate() {
    assert(only_zero_one(0) == 10);
    assert(only_zero_one(1) == 20);
    assert(only_zero_one(2) == -1);
}

void demo_expert() {
#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
    // Feature exists; we still never invoke it on a reachable path.
    static_assert(__cpp_lib_unreachable >= 202202L);
#endif
    assert(std::string{color_name(9)} == "unknown");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section06/std_unreachable_cpp23", run>;

}  // namespace
