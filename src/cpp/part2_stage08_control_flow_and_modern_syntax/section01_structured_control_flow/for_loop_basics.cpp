// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : for_loop_basics
// Topic id : part2/stage08/section01/for_loop_basics
//
// Covers: classic for, break/continue, nested loops

#include "learn/topic_registry.hpp"

#include <vector>

namespace {

int sum_n(int n) {
    int s = 0;
    for (int i = 1; i <= n; ++i) {
        s += i;
    }
    return s;
}

int sum_even(const std::vector<int>& v) {
    int s = 0;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (v[i] % 2 != 0) {
            continue;
        }
        s += v[i];
    }
    return s;
}

void demo_basics() {
    LEARN_CHECK(sum_n(10) == 55);
    LEARN_CHECK(sum_n(0) == 0);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    LEARN_CHECK(sum_even(v) == 12);

    int count = 0;
    for (int i = 0; i < 10; ++i) {
        if (i == 5) {
            break;
        }
        ++count;
    }
    LEARN_CHECK(count == 5);
}

void demo_expert() {
    int found = -1;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * j == 12) {
                found = i * 10 + j;
                break;
            }
        }
        if (found >= 0) {
            break;
        }
    }
    LEARN_CHECK(found == 34 || found == 43);  // 3*4 or 4*3
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/for_loop_basics", run>;

}  // namespace
