// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_regex
// Topic id : part2/stage10/section11_other_stl/std_regex
//
// Covers: std::regex match search iterate

#include "learn/topic_registry.hpp"

#include <regex>
#include <string>

namespace {

void demo_basics() {
    std::regex re{R"(\d+)"};
    LEARN_CHECK(std::regex_match("123", re));
    LEARN_CHECK(!std::regex_match("12a", re));
}

void demo_intermediate() {
    std::string s = "id=42;name=bob";
    std::regex re{R"(id=(\d+))"};
    std::smatch m;
    LEARN_CHECK(std::regex_search(s, m, re));
    LEARN_CHECK(m[1] == "42");
}

void demo_expert() {
    std::string s = "a1b22c333";
    std::regex re{R"(\d+)"};
    auto begin = std::sregex_iterator(s.begin(), s.end(), re);
    auto end = std::sregex_iterator{};
    int n = 0;
    int sum = 0;
    for (auto it = begin; it != end; ++it) {
        ++n;
        sum += std::stoi(it->str());
    }
    LEARN_CHECK(n == 3);
    LEARN_CHECK(sum == 1 + 22 + 333);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11_other_stl/std_regex", run>;

}  // namespace
