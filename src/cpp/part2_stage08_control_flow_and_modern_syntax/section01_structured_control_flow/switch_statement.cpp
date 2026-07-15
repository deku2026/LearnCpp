// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : switch_statement
// Topic id : part2/stage08/section01/switch_statement
//
// Covers: switch, case, default, break, intentional fallthrough marker

#include "learn/topic_registry.hpp"

#include <string>

namespace {

const char* day_name(int d) {
    switch (d) {
        case 1:
            return "mon";
        case 2:
            return "tue";
        case 3:
            return "wed";
        case 4:
            return "thu";
        case 5:
            return "fri";
        case 6:
            return "sat";
        case 7:
            return "sun";
        default:
            return "unknown";
    }
}

int bucket(int x) {
    switch (x) {
        case 0:
        case 1:
        case 2:
            return 0;
        case 3:
        case 4:
            return 1;
        default:
            return 2;
    }
}

void demo_basics() {
    LEARN_CHECK(std::string{day_name(1)} == "mon");
    LEARN_CHECK(std::string{day_name(7)} == "sun");
    LEARN_CHECK(std::string{day_name(0)} == "unknown");
}

void demo_intermediate() {
    LEARN_CHECK(bucket(0) == 0);
    LEARN_CHECK(bucket(2) == 0);
    LEARN_CHECK(bucket(3) == 1);
    LEARN_CHECK(bucket(9) == 2);
}

void demo_expert() {
    enum class Color { Red, Green, Blue };
    auto name = [](Color c) {
        switch (c) {
            case Color::Red:
                return "r";
            case Color::Green:
                return "g";
            case Color::Blue:
                return "b";
        }
        return "?";
    };
    LEARN_CHECK(std::string{name(Color::Green)} == "g");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/switch_statement", run>;

}  // namespace
