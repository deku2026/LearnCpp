// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section02_variadic_functions
// Item     : initializer_list_parameter
// Topic id : part2/stage03/section02/initializer_list_parameter
//
// Covers: std::initializer_list for same-type multi-arg APIs

#include "learn/topic_registry.hpp"

#include <initializer_list>
#include <numeric>
#include <vector>

namespace {

int sum(std::initializer_list<int> nums) {
    int total = 0;
    for (int n : nums) {
        total += n;
    }
    return total;
}

std::size_t count(std::initializer_list<int> nums) {
    return nums.size();
}

std::vector<int> to_vector(std::initializer_list<int> nums) {
    return std::vector<int>(nums);
}

void demo_basics() {
    LEARN_CHECK(sum({1, 2, 3}) == 6);
    LEARN_CHECK(sum({10, 20, 30, 40}) == 100);
    LEARN_CHECK(sum({}) == 0);
}

void demo_intermediate() {
    LEARN_CHECK(count({1, 2, 3, 4}) == 4);
    auto v = to_vector({7, 8, 9});
    LEARN_CHECK(v.size() == 3);
    LEARN_CHECK(v[0] == 7 && v[2] == 9);
}

void demo_expert() {
    // Elements must share a common type (or convert to T).
    std::initializer_list<int> nums{1, 2, 3, 4};
    const auto total = std::accumulate(nums.begin(), nums.end(), 0);
    LEARN_CHECK(total == 10);
    LEARN_CHECK(sum(nums) == 10);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section02/initializer_list_parameter", run>;

}  // namespace
