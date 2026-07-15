// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : feature_macro_probe_in_ci_cpp23
// Topic id : part4/section06/feature_macro_probe_in_ci_cpp23
//
// Covers: probe __cpp_lib_* in CI for feature availability

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__has_include)
#if __has_include(<format>)
#include <format>
#endif
#endif
namespace {

void demo_basics() {
    // CI can compile a small probe TU printing feature macros
#if defined(__cplusplus)
    LEARN_CHECK(__cplusplus >= 201703L);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_format) && __has_include(<format>)
    LEARN_CHECK(__cpp_lib_format >= 201907L || __cpp_lib_format < 201907L);
#endif
    LEARN_CHECK(true);
}

void demo_expert() {
    // Gate optional demos; fail CI only for required baseline features
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/feature_macro_probe_in_ci_cpp23", run>;

}  // namespace
