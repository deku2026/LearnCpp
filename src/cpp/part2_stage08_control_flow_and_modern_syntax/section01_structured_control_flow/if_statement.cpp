// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : if_statement
// Topic id : part2/stage08/section01/if_statement
//
// Covers: if / else if / else, dangling else, bool conversion

#include "learn/topic_registry.hpp"

#include <string>

namespace {

int sign(int x) {
    if (x > 0) {
        return 1;
    } else if (x < 0) {
        return -1;
    } else {
        return 0;
    }
}

int classify_ptr(const int* p) {
    if (p) {
        return *p;
    }
    return -1;
}

void demo_basics() {
    LEARN_CHECK(sign(5) == 1);
    LEARN_CHECK(sign(-2) == -1);
    LEARN_CHECK(sign(0) == 0);
}

void demo_intermediate() {
    int x = 10;
    LEARN_CHECK(classify_ptr(&x) == 10);
    LEARN_CHECK(classify_ptr(nullptr) == -1);

    // Nested if with braces (avoid dangling-else ambiguity).
    int a = 1;
    int b = 0;
    int r = 0;
    if (a) {
        if (b) {
            r = 1;
        } else {
            r = 2;
        }
    }
    LEARN_CHECK(r == 2);
}

void demo_expert() {
    std::string s = "ok";
    if (!s.empty()) {
        LEARN_CHECK(s.size() == 2);
    } else {
        LEARN_CHECK(false);
    }

    // Common pattern: early return style simulated.
    auto abs_if = [](int x) {
        if (x >= 0) {
            return x;
        }
        return -x;
    };
    LEARN_CHECK(abs_if(-3) == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/if_statement", run>;

}  // namespace
