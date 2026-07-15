// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section02_associative_containers
// Item     : contains_member_cpp20
// Topic id : part2/stage10/section02_associative_containers/contains_member_cpp20
//
// Covers: C++20 contains() on associative containers

#include "learn/topic_registry.hpp"

#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>

namespace {

void demo_basics() {
    std::map<std::string, int> m{{"a", 1}};
    LEARN_CHECK(m.contains("a"));
    LEARN_CHECK(!m.contains("b"));
}

void demo_intermediate() {
    std::set<int> s{1, 2, 3};
    LEARN_CHECK(s.contains(2));
    std::unordered_map<int, int> um{{5, 50}};
    LEARN_CHECK(um.contains(5));
    LEARN_CHECK(!um.contains(6));
}

void demo_expert() {
    std::map<std::string, int, std::less<>> m{{"hello", 1}};
    LEARN_CHECK(m.contains(std::string_view{"hello"}));
    LEARN_CHECK(!m.contains(std::string_view{"world"}));
    // Prefer contains over find != end for readability
    LEARN_CHECK(m.find("hello") != m.end());
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
    ::learn::topic<"part2/stage10/section02_associative_containers/contains_member_cpp20", run>;

}  // namespace
