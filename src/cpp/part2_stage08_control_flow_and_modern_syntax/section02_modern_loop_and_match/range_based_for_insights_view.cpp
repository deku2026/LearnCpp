// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : range_based_for_insights_view
// Topic id : part2/stage08/section02/range_based_for_insights_view
//
// Covers: range-for expansion (auto / auto& / auto&&), container iteration

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3};
    int s = 0;
    for (int x : v) {
        s += x;
    }
    assert(s == 6);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3};
    for (int& x : v) {
        x *= 2;
    }
    assert(v[0] == 2 && v[1] == 4 && v[2] == 6);

    // auto&& binds to element correctly for generic code.
    int s = 0;
    for (auto&& e : v) {
        s += e;
    }
    assert(s == 12);
}

void demo_expert() {
    // Copy vs reference for heavy elements.
    std::vector<std::string> words{"a", "bb", "ccc"};
    std::size_t total = 0;
    for (const auto& w : words) {
        total += w.size();
    }
    assert(total == 6);

    // C-array also works.
    int a[] = {4, 5, 6};
    int p = 1;
    for (int x : a) {
        p *= x;
    }
    assert(p == 120);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/range_based_for_insights_view", run>;

}  // namespace
