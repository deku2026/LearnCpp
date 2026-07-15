// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section09_feature_test_macros
// Item     : cpp_library_feature_macros
// Topic id : part2/stage14/section09/cpp_library_feature_macros
//
// Covers: __cpp_lib_* library feature-test macros

#include "learn/topic_registry.hpp"

#include <cassert>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_lib_optional)
    assert(__cpp_lib_optional >= 201606L);
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_jthread)
    assert(__cpp_lib_jthread >= 201911L);
#else
    assert(true);
#endif
#if defined(__cpp_lib_span)
    assert(__cpp_lib_span >= 202002L);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_expected)
    assert(__cpp_lib_expected >= 202202L);
#else
    assert(true);
#endif
#if defined(__cpp_lib_generator)
    assert(__cpp_lib_generator >= 202207L);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section09/cpp_library_feature_macros", run>;

}  // namespace
