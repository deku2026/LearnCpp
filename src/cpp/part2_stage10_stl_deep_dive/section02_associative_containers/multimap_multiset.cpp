// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : multimap_multiset
// Topic id : part2/stage10/section02_associative_containers/multimap_multiset
//
// Covers: multimap/multiset allow duplicate keys

#include "learn/topic_registry.hpp"

#include <cassert>
#include <map>
#include <set>
#include <string>

namespace {

void demo_basics() {
    std::multiset<int> ms{1, 2, 2, 3};
    assert(ms.size() == 4);
    assert(ms.count(2) == 2);
    std::multimap<std::string, int> mm{{"a", 1}, {"a", 2}};
    assert(mm.count("a") == 2);
}

void demo_intermediate() {
    std::multimap<int, char> mm{{1, 'a'}, {1, 'b'}, {2, 'c'}};
    auto range = mm.equal_range(1);
    int n = 0;
    for (auto it = range.first; it != range.second; ++it) {
        ++n;
    }
    assert(n == 2);
}

void demo_expert() {
    std::multiset<int> ms{5, 1, 3, 1, 4};
    auto it = ms.find(1);
    assert(it != ms.end());
    ms.erase(it);  // erase one of the 1s
    assert(ms.count(1) == 1);
    assert(ms.size() == 4);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section02_associative_containers/multimap_multiset", run>;

}  // namespace
