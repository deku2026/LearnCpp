// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : unordered_map_set
// Topic id : part2/stage10/section02_associative_containers/unordered_map_set
//
// Covers: std::unordered_map / unordered_set hash tables average O(1)

#include "learn/topic_registry.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace {

void demo_basics() {
    std::unordered_map<std::string, int> um{{"a", 1}, {"b", 2}};
    LEARN_CHECK(um["a"] == 1);
    LEARN_CHECK(um.count("b") == 1);
    std::unordered_set<int> us{1, 2, 2, 3};
    LEARN_CHECK(us.size() == 3);
}

void demo_intermediate() {
    std::unordered_map<int, int> um;
    um.reserve(100);
    for (int i = 0; i < 50; ++i) {
        um[i] = i * i;
    }
    LEARN_CHECK(um.size() == 50);
    LEARN_CHECK(um.at(7) == 49);
    um.erase(7);
    LEARN_CHECK(um.find(7) == um.end());
}

void demo_expert() {
    std::unordered_map<std::string, int> um;
    um.max_load_factor(0.5f);
    um.rehash(32);
    LEARN_CHECK(um.bucket_count() >= 32);
    um["x"] = 1;
    LEARN_CHECK(um.load_factor() <= um.max_load_factor() + 0.01f);
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
    ::learn::topic<"part2/stage10/section02_associative_containers/unordered_map_set", run>;

}  // namespace
