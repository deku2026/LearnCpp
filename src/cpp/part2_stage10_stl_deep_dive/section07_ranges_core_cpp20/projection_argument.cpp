// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : projection_argument
// Topic id : part2/stage10/section07_ranges_core_cpp20/projection_argument
//
// Covers: ranges algorithms with projection

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

namespace {

struct Person {
    std::string name;
    int age;
};

void demo_basics() {
    std::vector<Person> people{{"Bob", 30}, {"Ann", 25}, {"Carl", 40}};
    std::ranges::sort(people, {}, &Person::age);
    LEARN_CHECK(people.front().name == "Ann");
    LEARN_CHECK(people.back().name == "Carl");
}

void demo_intermediate() {
    std::vector<Person> people{{"Bob", 30}, {"Ann", 25}};
    auto it = std::ranges::find(people, 30, &Person::age);
    LEARN_CHECK(it != people.end());
    LEARN_CHECK(it->name == "Bob");
}

void demo_expert() {
    std::vector<std::pair<int, std::string>> v{{2, "b"}, {1, "a"}};
    std::ranges::sort(v, std::ranges::greater{}, &std::pair<int, std::string>::first);
    LEARN_CHECK(v.front().first == 2);
    LEARN_CHECK(std::ranges::max(v, {}, &std::pair<int, std::string>::first).first == 2);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/projection_argument", run>;

}  // namespace
