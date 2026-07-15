// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : containers_headers
// Topic id : part3/section08/containers_headers
//
// Covers: headers map: vector array deque list map set unordered_* stack queue

#include "learn/topic_registry.hpp"

#include <array>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1};
    std::array<int, 2> a{{2, 3}};
    LEARN_CHECK(v[0] + a[0] == 3);
}

void demo_intermediate() {
    std::map<int, int> m{{1, 1}};
    std::set<int> s{1, 2};
    LEARN_CHECK(m[1] == 1 && s.count(2) == 1);
}

void demo_expert() {
    std::unordered_map<int, int> u{{1, 9}};
    std::stack<int> st;
    st.push(1);
    LEARN_CHECK(u[1] == 9 && st.top() == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/containers_headers", run>;

}  // namespace
