// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section09_feature_test_macros
// Item     : cpp_library_feature_macros
// Topic id : part2/stage14/section09/cpp_library_feature_macros
//
// Covers: __cpp_lib_* library feature-test macros

#include "learn/topic_registry.hpp"

#include <version>

#if defined(__has_include)
#if __has_include(<expected>)
#include <expected>
#endif
#if __has_include(<generator>)
#include <generator>
#endif
#endif
namespace {

void demo_basics() {
#if defined(__cpp_lib_optional)
    LEARN_CHECK(__cpp_lib_optional >= 201606L);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_jthread)
    LEARN_CHECK(__cpp_lib_jthread >= 201911L);
#else
    LEARN_CHECK(true);
#endif
#if defined(__cpp_lib_span)
    LEARN_CHECK(__cpp_lib_span >= 202002L);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_expected) && __has_include(<expected>)
    LEARN_CHECK(__cpp_lib_expected >= 202202L);
#else
    LEARN_CHECK(true);
#endif
#if defined(__cpp_lib_generator) && __has_include(<generator>)
    LEARN_CHECK(__cpp_lib_generator >= 202207L);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section09/cpp_library_feature_macros", run>;

}  // namespace
