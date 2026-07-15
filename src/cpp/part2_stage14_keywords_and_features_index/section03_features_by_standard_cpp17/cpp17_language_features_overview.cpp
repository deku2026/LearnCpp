// LearnCpp topic example
// Doc      : part2-stage14-keywords-and-features-index.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section03_features_by_standard_cpp17
// Item     : cpp17_language_features_overview
// Topic id : part2/stage14/section03/cpp17_language_features_overview
//
// Covers: C++17 language feature sampler

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>
#include <tuple>
#include <utility>

namespace {

void demo_basics() {
    if (int x = 3; x > 0) {
        LEARN_CHECK(x == 3);
    }
    std::string s = "hello";
    std::string_view sv = s;
    LEARN_CHECK(sv.size() == 5);
}

void demo_intermediate() {
    auto t = std::tuple{1, 2};
    auto [a, b] = t;
    LEARN_CHECK(a == 1 && b == 2);
    if constexpr (sizeof(int) >= 4) {
        LEARN_CHECK(true);
    }
}

void demo_expert() {
    [[maybe_unused]] int x = 1;
    auto f = []() constexpr { return 5; };
    static_assert(f() == 5);
    void* p = nullptr;
    if (p == nullptr) {
        LEARN_CHECK(true);
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage14/section03/cpp17_language_features_overview", run>;

}  // namespace
