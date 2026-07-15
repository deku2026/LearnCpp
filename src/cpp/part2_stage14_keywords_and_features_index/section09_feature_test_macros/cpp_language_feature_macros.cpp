// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section09_feature_test_macros
// Item     : cpp_language_feature_macros
// Topic id : part2/stage14/section09/cpp_language_feature_macros
//
// Covers: __cpp_* language feature-test macros

#include "learn/topic_registry.hpp"

#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_concepts)
    LEARN_CHECK(__cpp_concepts >= 201907L);
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_impl_coroutine)
    LEARN_CHECK(__cpp_impl_coroutine >= 201902L);
#elif defined(__cpp_coroutines)
    LEARN_CHECK(__cpp_coroutines >= 201703L);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
    // Sample a few language macros; values document minimum standard paper.
#if defined(__cpp_lib_is_constant_evaluated) || defined(__cpp_constexpr)
    LEARN_CHECK(true);
#else
    LEARN_CHECK(true);
#endif
#if defined(__cpp_modules)
    (void)__cpp_modules;
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section09/cpp_language_feature_macros", run>;

}  // namespace
