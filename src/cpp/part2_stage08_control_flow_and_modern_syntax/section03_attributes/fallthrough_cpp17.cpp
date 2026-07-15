// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : fallthrough_cpp17
// Topic id : part2/stage08/section03/fallthrough_cpp17
//
// Covers: [[fallthrough]] intentional switch fallthrough

#include "learn/topic_registry.hpp"

#include <string>

namespace {

std::string describe(int code) {
    std::string s;
    switch (code) {
        case 1:
            s += "one";
            [[fallthrough]];
        case 2:
            s += "+twoish";
            break;
        case 3:
            s = "three";
            break;
        default:
            s = "other";
            break;
    }
    return s;
}

int points(char grade) {
    switch (grade) {
        case 'A':
            return 4;
        case 'B':
            return 3;
        case 'C':
            return 2;
        case 'D':
        case 'F':
            return 0;
        default:
            return -1;
    }
}

void demo_basics() {
    LEARN_CHECK(describe(1) == "one+twoish");
    LEARN_CHECK(describe(2) == "+twoish");
    LEARN_CHECK(describe(3) == "three");
}

void demo_intermediate() {
    LEARN_CHECK(points('A') == 4);
    LEARN_CHECK(points('D') == 0);
    LEARN_CHECK(points('Z') == -1);
}

void demo_expert() {
    // Fallthrough without attribute may warn; with attribute documents intent.
    LEARN_CHECK(describe(0) == "other");
    LEARN_CHECK(describe(2).find("two") != std::string::npos);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/fallthrough_cpp17", run>;

}  // namespace
