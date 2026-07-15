// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : sequence_associative_unordered_adapter_overview
// Topic id : part3/section02/sequence_associative_unordered_adapter_overview
//
// Covers: vector/list/map/unordered_map/stack/queue overview

#include "learn/topic_registry.hpp"

#include <list>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3};
    LEARN_CHECK(v.size() == 3);
    LEARN_CHECK(v.back() == 3);
    std::list<int> lst{4, 5};
    LEARN_CHECK(lst.front() == 4);
}

void demo_intermediate() {
    std::map<std::string, int> m{{"a", 1}, {"b", 2}};
    LEARN_CHECK(m["a"] == 1);
    std::unordered_map<int, int> u{{1, 10}};
    LEARN_CHECK(u[1] == 10);
}

void demo_expert() {
    std::stack<int> st;
    st.push(1);
    st.push(2);
    LEARN_CHECK(st.top() == 2);
    st.pop();
    LEARN_CHECK(st.top() == 1);
    std::queue<int> q;
    q.push(9);
    LEARN_CHECK(q.front() == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/sequence_associative_unordered_adapter_overview", run>;

}  // namespace
