// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section09_feature_test_macros
// Item     : has_include_and_has_cpp_attribute
// Topic id : part2/stage14/section09/has_include_and_has_cpp_attribute
//
// Covers: __has_include and __has_cpp_attribute

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

void demo_basics() {
#if defined(__has_include)
#if __has_include(<optional>)
    assert(true);
#else
    assert(false);
#endif
#else
    assert(true);
#endif
}

void demo_intermediate() {
#if defined(__has_include)
#if __has_include(<version>)
    assert(true);
#endif
#if __has_include(<generator>)
    assert(true);
#endif
#endif
    assert(true);
}

void demo_expert() {
#if defined(__has_cpp_attribute)
#if __has_cpp_attribute(likely)
    int x = 0;
    if (true) [[likely]] {
        x = 1;
    }
    assert(x == 1);
#else
    int x = 1;
    assert(x == 1);
#endif
#if __has_cpp_attribute(nodiscard)
    struct S {
        [[nodiscard]] int f() const { return 2; }
    };
    assert(S{}.f() == 2);
#endif
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section09/has_include_and_has_cpp_attribute", run>;

}  // namespace
