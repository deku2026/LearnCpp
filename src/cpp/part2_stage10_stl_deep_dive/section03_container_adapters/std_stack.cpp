// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section03_container_adapters
// Item     : std_stack
// Topic id : part2/stage10/section03_container_adapters/std_stack
//
// Covers: std::stack LIFO adapter, default deque

#include "learn/topic_registry.hpp"

#include <stack>
#include <vector>

namespace {

void demo_basics() {
    std::stack<int> st;
    st.push(1);
    st.push(2);
    LEARN_CHECK(st.top() == 2);
    st.pop();
    LEARN_CHECK(st.top() == 1);
    LEARN_CHECK(st.size() == 1);
}

void demo_intermediate() {
    std::stack<int, std::vector<int>> st;
    st.push(10);
    st.push(20);
    LEARN_CHECK(!st.empty());
    st.pop();
    LEARN_CHECK(st.top() == 10);
}

void demo_expert() {
    std::stack<int> a;
    std::stack<int> b;
    a.push(1);
    a.push(2);
    b.push(9);
    a.swap(b);
    LEARN_CHECK(a.top() == 9);
    LEARN_CHECK(b.top() == 2);
    LEARN_CHECK(a.size() == 1);
    LEARN_CHECK(b.size() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section03_container_adapters/std_stack", run>;

}  // namespace
