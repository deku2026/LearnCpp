// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section04_features_by_standard_cpp20
// Item     : cpp20_language_features_overview
// Topic id : part2/stage14/section04/cpp20_language_features_overview
//
// Covers: C++20 language feature sampler

#include "learn/topic_registry.hpp"

#include <concepts>
#include <span>
#include <string>
#include <version>

namespace {

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template <Addable T>
T add(T a, T b) {
    return a + b;
}

void demo_basics() {
    LEARN_CHECK(add(1, 2) == 3);
#if defined(__cpp_designated_initializers)
    struct P {
        int x;
        int y;
    };
    P p{.x = 1, .y = 2};
    LEARN_CHECK(p.x == 1 && p.y == 2);
#endif
}

void demo_intermediate() {
    int a[] = {1, 2, 3, 4};
    std::span<int> s{a};
    LEARN_CHECK(s.size() == 4);
    LEARN_CHECK(s[1] == 2);
}

void demo_expert() {
#if defined(__cpp_impl_three_way_comparison)
    LEARN_CHECK((1 <=> 2) < 0);
#else
    LEARN_CHECK(1 < 2);
#endif
    const char* msg = "ok";
    LEARN_CHECK(std::string{msg} == "ok");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section04/cpp20_language_features_overview", run>;

}  // namespace
