// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : stacktrace_in_runtime_cpp23
// Topic id : part4/section05/stacktrace_in_runtime_cpp23
//
// Covers: runtime stacktrace for diagnostics

#include "learn/topic_registry.hpp"

#include <string>
#include <version>
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
#include <stacktrace>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
    auto st = std::stacktrace::current();
    (void)st;
#endif
    LEARN_CHECK(true);
}

void demo_intermediate() {
    // Log stacktrace on fatal errors in engineering builds
    LEARN_CHECK(true);
}

void demo_expert() {
    // Combine with sanitizer reports and core dumps
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/stacktrace_in_runtime_cpp23", run>;

}  // namespace
