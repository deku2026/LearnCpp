// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section02_features_by_standard_cpp14
// Item     : cpp14_language_features_overview
// Topic id : part2/stage14/section02/cpp14_language_features_overview
//
// Covers: C++14 language feature sampler

#include "learn/topic_registry.hpp"

#include <memory>
#include <type_traits>
#include <utility>

namespace {

void demo_basics() {
    auto add = [](auto a, auto b) { return a + b; };
    LEARN_CHECK(add(1, 2) == 3);
    LEARN_CHECK(add(1.5, 2.5) == 4.0);
}

void demo_intermediate() {
    auto f = [](int x) {
        auto g = [x] { return x * 2; };
        return g;
    };
    LEARN_CHECK(f(5)() == 10);
    int binary = 0b1010;
    LEARN_CHECK(binary == 10);
}

void demo_expert() {
    constexpr auto square = [](int x) { return x * x; };
    static_assert(square(4) == 16);
    auto p = std::make_unique<int>(9);
    LEARN_CHECK(*p == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section02/cpp14_language_features_overview", run>;

}  // namespace
