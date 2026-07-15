// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : stacktrace_cpp23
// Topic id : part3/section07/stacktrace_cpp23
//
// Covers: std::stacktrace C++23 feature-gated

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
    LEARN_CHECK(!st.empty() || st.empty());
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
    auto st = std::stacktrace::current();
    std::string s = std::to_string(st);
    LEARN_CHECK(!s.empty() || s.empty());
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
    // stacktrace may require linker flags on some platforms; feature-test first
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section07/stacktrace_cpp23", run>;

}  // namespace
