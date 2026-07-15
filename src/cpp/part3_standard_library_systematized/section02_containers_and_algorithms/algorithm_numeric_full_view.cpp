// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : algorithm_numeric_full_view
// Topic id : part3/section02/algorithm_numeric_full_view
//
// Covers: std::algorithm + numeric: sort, find, accumulate, iota

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <numeric>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{3, 1, 2};
    std::sort(v.begin(), v.end());
    LEARN_CHECK((v == std::vector<int>{1, 2, 3}));
    LEARN_CHECK(std::find(v.begin(), v.end(), 2) != v.end());
}

void demo_intermediate() {
    std::vector<int> v(5);
    std::iota(v.begin(), v.end(), 1);
    LEARN_CHECK(v.front() == 1 && v.back() == 5);
    int sum = std::accumulate(v.begin(), v.end(), 0);
    LEARN_CHECK(sum == 15);
}

void demo_expert() {
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{1, 2, 3};
    LEARN_CHECK(std::equal(a.begin(), a.end(), b.begin()));
    std::vector<int> out(3);
    std::partial_sum(a.begin(), a.end(), out.begin());
    LEARN_CHECK((out == std::vector<int>{1, 3, 6}));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/algorithm_numeric_full_view", run>;

}  // namespace
