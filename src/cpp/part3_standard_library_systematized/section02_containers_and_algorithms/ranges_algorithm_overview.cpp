// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : ranges_algorithm_overview
// Topic id : part3/section02/ranges_algorithm_overview
//
// Covers: std::ranges algorithms and views pipeline

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{3, 1, 2};
    std::ranges::sort(v);
    LEARN_CHECK(std::ranges::is_sorted(v));
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4, 5};
    auto even = v | std::views::filter([](int x) { return x % 2 == 0; });
    std::vector<int> got;
    for (int x : even) {
        got.push_back(x);
    }
    LEARN_CHECK((got == std::vector<int>{2, 4}));
}

void demo_expert() {
    std::vector<int> v{1, 2, 3};
    auto squared = v | std::views::transform([](int x) { return x * x; });
    int sum = 0;
    for (int x : squared) {
        sum += x;
    }
    LEARN_CHECK(sum == 14);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/ranges_algorithm_overview", run>;

}  // namespace
