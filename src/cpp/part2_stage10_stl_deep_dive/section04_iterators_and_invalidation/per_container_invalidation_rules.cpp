// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : per_container_invalidation_rules
// Topic id : part2/stage10/section04_iterators_and_invalidation/per_container_invalidation_rules
//
// Covers: iterator invalidation rules per container family

#include "learn/topic_registry.hpp"

#include <list>
#include <map>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4};
    v.reserve(16);
    auto it = v.begin();
    v.push_back(5);  // no realloc
    LEARN_CHECK(*it == 1);
    it = v.erase(v.begin() + 1);
    LEARN_CHECK(*it == 3);
}

void demo_intermediate() {
    std::list<int> L{1, 2, 3};
    auto a = L.begin();
    auto b = std::next(a);
    L.erase(b);  // only erased iterator invalidated
    LEARN_CHECK(*a == 1);
    LEARN_CHECK(L.size() == 2);
}

void demo_expert() {
    std::map<int, int> m{{1, 1}, {2, 2}, {3, 3}};
    auto it1 = m.find(1);
    auto it3 = m.find(3);
    m.erase(2);  // only key 2 invalidated
    LEARN_CHECK(it1->second == 1);
    LEARN_CHECK(it3->second == 3);
    LEARN_CHECK(m.size() == 2);
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
    ::learn::topic<"part2/stage10/section04_iterators_and_invalidation/per_container_invalidation_rules", run>;

}  // namespace
