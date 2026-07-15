// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : ordered_map_set
// Topic id : part2/stage10/section02_associative_containers/ordered_map_set
//
// Covers: std::map / std::set red-black tree, ordered iteration O(log n)

#include "learn/topic_registry.hpp"

#include <map>
#include <set>
#include <string>

namespace {

void demo_basics() {
    std::map<std::string, int> m{{"banana", 2}, {"apple", 1}};
    LEARN_CHECK(m.begin()->first == "apple");
    LEARN_CHECK(m["banana"] == 2);
    std::set<int> s{3, 1, 4, 1, 5};
    LEARN_CHECK(s.size() == 4);
    LEARN_CHECK(*s.begin() == 1);
}

void demo_intermediate() {
    std::map<int, int> m;
    m.insert({1, 10});
    m.emplace(2, 20);
    LEARN_CHECK(m.count(1) == 1);
    LEARN_CHECK(m.find(3) == m.end());
    auto [it, ok] = m.insert({1, 99});
    LEARN_CHECK(!ok);
    LEARN_CHECK(it->second == 10);
}

void demo_expert() {
    std::map<int, int> m{{1, 1}, {2, 2}, {3, 3}};
    auto it = m.find(2);
    LEARN_CHECK(it != m.end());
    m.erase(it);
    LEARN_CHECK(m.size() == 2);
    LEARN_CHECK(m.lower_bound(2)->first == 3);
    LEARN_CHECK(m.upper_bound(1)->first == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section02_associative_containers/ordered_map_set", run>;

}  // namespace
