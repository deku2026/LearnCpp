// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : range_based_for_with_initializer_cpp20
// Topic id : part2/stage08/section02/range_based_for_with_initializer_cpp20
//
// Covers: range-for with init-statement (C++20)

#include "learn/topic_registry.hpp"

#include <map>
#include <string>
#include <vector>

namespace {

void demo_basics() {
    int sum = 0;
    for (std::vector<int> v{1, 2, 3}; int x : v) {
        sum += x;
    }
    LEARN_CHECK(sum == 6);
}

void demo_intermediate() {
    std::map<std::string, int> m{{"a", 1}, {"b", 2}};
    int total = 0;
    for (auto it = m.begin(); const auto& [k, v] : m) {
        (void)it;
        (void)k;
        total += v;
    }
    LEARN_CHECK(total == 3);
}

void demo_expert() {
    // Init keeps temporary alive for the whole loop (related to C++23 range-for extension).
    int n = 0;
    for (auto data = std::vector<int>{10, 20, 30}; int x : data) {
        n += x;
    }
    LEARN_CHECK(n == 60);

    int count = 0;
    for (int i = 0; char c : std::string{"xy"}) {
        (void)i;
        ++count;
        (void)c;
    }
    LEARN_CHECK(count == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/range_based_for_with_initializer_cpp20", run>;

}  // namespace
