// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_iterator
// Topic id : part6/b/section03/dangling_iterator
//
// Covers: iterators invalidate on reallocation; re-get after mutate

#include "learn/topic_registry.hpp"

#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3};
    auto it = v.begin();
    LEARN_CHECK(*it == 1);
}

void demo_intermediate() {
    std::vector<int> v{1};
    v.reserve(100);
    auto it = v.begin();
    v.push_back(2);  // no reallocation due to reserve
    LEARN_CHECK(*it == 1);
    LEARN_CHECK(v.size() == 2);
}

void demo_expert() {
    std::vector<int> v{1, 2};
    // After potential reallocation, old iterators are invalid — reacquire.
    v.push_back(3);
    auto it = v.begin();
    LEARN_CHECK(*it == 1);
    LEARN_CHECK(v.back() == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_iterator", run>;

}  // namespace
