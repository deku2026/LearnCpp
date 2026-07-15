// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section05_features_by_standard_cpp23_language
// Item     : cpp23_language_feature_checklist
// Topic id : part2/stage14/section05/cpp23_language_feature_checklist
//
// Covers: C++23 language feature checklist samples

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <utility>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    struct S {
        int n = 1;
        int get(this const S& self) { return self.n; }
    };
    LEARN_CHECK(S{}.get() == 1);
#else
    struct S {
        int n = 1;
        int get() const { return n; }
    };
    LEARN_CHECK(S{}.get() == 1);
#endif
}

void demo_intermediate() {
#if defined(__cpp_if_consteval)
    auto f = []() {
        if consteval {
            return 1;
        } else {
            return 2;
        }
    };
    LEARN_CHECK(f() == 2);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_size_t_suffix)
    auto n = 10uz;
    LEARN_CHECK(n == 10);
#else
    std::size_t n = 10;
    LEARN_CHECK(n == 10);
#endif
    static_assert(std::is_same_v<std::size_t, decltype(sizeof(0))>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section05/cpp23_language_feature_checklist", run>;

}  // namespace
