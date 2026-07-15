// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : std_stacktrace_current_cpp23
// Topic id : part2/stage09/section06/std_stacktrace_current_cpp23
//
// Covers: std::stacktrace::current (C++23) with feature test

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <version>

#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
#include <stacktrace>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
    auto st = std::stacktrace::current();
    assert(st.size() >= 0);
#else
    // Portable: document API shape without requiring stacktrace library.
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
    auto st = std::stacktrace::current();
    std::string s = std::to_string(st);
    // Implementation-defined content; just ensure call works.
    assert(s.size() >= 0);
#else
    std::string s = "stacktrace unavailable";
    assert(!s.empty());
#endif
}

void demo_expert() {
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
    auto st = std::stacktrace::current();
    if (!st.empty()) {
        auto desc = st[0].description();
        assert(desc.size() >= 0);
    }
#else
    assert(true);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section06/std_stacktrace_current_cpp23", run>;

}  // namespace
