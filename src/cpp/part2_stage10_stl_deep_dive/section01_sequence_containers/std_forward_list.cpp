// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_forward_list
// Topic id : part2/stage10/section01_sequence_containers/std_forward_list
//
// Covers: std::forward_list singly-linked, before_begin insert_after

#include "learn/topic_registry.hpp"

#include <forward_list>

namespace {

void demo_basics() {
    std::forward_list<int> fl{1, 2, 3};
    LEARN_CHECK(fl.front() == 1);
    fl.push_front(0);
    LEARN_CHECK(fl.front() == 0);
}

void demo_intermediate() {
    std::forward_list<int> fl{1, 3};
    auto it = fl.before_begin();
    fl.insert_after(it, 0);
    LEARN_CHECK(fl.front() == 0);
    auto p = fl.begin();  // 0
    ++p;                  // 1
    fl.insert_after(p, 2);
    // 0,1,2,3
    int sum = 0;
    for (int x : fl) {
        sum += x;
    }
    LEARN_CHECK(sum == 6);
}

void demo_expert() {
    std::forward_list<int> fl{5, 1, 4, 2, 3};
    fl.sort();
    LEARN_CHECK(fl.front() == 1);
    fl.unique();
    fl.remove_if([](int x) { return x % 2 == 0; });
    int n = 0;
    for (int x : fl) {
        (void)x;
        ++n;
    }
    LEARN_CHECK(n == 3);  // 1,3,5
    fl.reverse();
    LEARN_CHECK(fl.front() == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01_sequence_containers/std_forward_list", run>;

}  // namespace
