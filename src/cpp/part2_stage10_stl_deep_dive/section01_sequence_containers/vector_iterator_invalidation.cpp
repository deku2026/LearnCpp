// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : vector_iterator_invalidation
// Topic id : part2/stage10/section01_sequence_containers/vector_iterator_invalidation
//
// Covers: vector iterator/pointer invalidation on reallocation and insert/erase

#include "learn/topic_registry.hpp"

#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3};
    v.reserve(16);
    int* p = &v[0];
    auto it = v.begin();
    v.push_back(4);  // no reallocation
    LEARN_CHECK(p == &v[0]);
    LEARN_CHECK(it == v.begin());
    LEARN_CHECK(*p == 1);
    LEARN_CHECK(*it == 1);
}

void demo_intermediate() {
    std::vector<int> v{10, 20, 30, 40};
    auto it = v.begin() + 1;
    LEARN_CHECK(*it == 20);
    it = v.erase(it);  // erase invalidates from erase point; returns next
    LEARN_CHECK(*it == 30);
    LEARN_CHECK((v == std::vector<int>{10, 30, 40}));
}

void demo_expert() {
    std::vector<int> v;
    v.reserve(2);
    v.push_back(1);
    v.push_back(2);
    const auto cap_before = v.capacity();
    int* old = &v[0];
    v.push_back(3);  // may reallocate
    if (v.capacity() > cap_before) {
        // old pointer is invalidated after reallocation; do not use it
        LEARN_CHECK(&v[0] != old || v.capacity() == cap_before);
    }
    LEARN_CHECK(v.size() == 3);
    LEARN_CHECK(v[0] == 1 && v[2] == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section01_sequence_containers/vector_iterator_invalidation", run>;

}  // namespace
