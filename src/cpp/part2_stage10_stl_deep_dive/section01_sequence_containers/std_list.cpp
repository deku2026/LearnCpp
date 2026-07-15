// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_list
// Topic id : part2/stage10/section01_sequence_containers/std_list
//
// Covers: std::list bidirectional linked list, splice, stable iterators

#include "learn/topic_registry.hpp"

#include <list>

namespace {

void demo_basics() {
    std::list<int> L{1, 2, 3};
    LEARN_CHECK(L.size() == 3);
    L.push_front(0);
    L.push_back(4);
    LEARN_CHECK(L.front() == 0);
    LEARN_CHECK(L.back() == 4);
}

void demo_intermediate() {
    std::list<int> L{1, 2, 3, 4};
    auto it = L.begin();
    ++it;
    L.insert(it, 99);
    LEARN_CHECK(*std::next(L.begin()) == 99);
    L.remove(99);
    LEARN_CHECK(L.size() == 4);
    L.reverse();
    LEARN_CHECK(L.front() == 4);
}

void demo_expert() {
    std::list<int> a{1, 2, 3};
    std::list<int> b{10, 20};
    auto keep = a.begin();
    ++keep;  // points to 2
    a.splice(a.end(), b);
    LEARN_CHECK(a.size() == 5);
    LEARN_CHECK(b.empty());
    LEARN_CHECK(*keep == 2);  // iterator not invalidated by splice into a
    a.sort();
    LEARN_CHECK(a.front() == 1);
    LEARN_CHECK(a.back() == 20);
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01_sequence_containers/std_list", run>;

}  // namespace
