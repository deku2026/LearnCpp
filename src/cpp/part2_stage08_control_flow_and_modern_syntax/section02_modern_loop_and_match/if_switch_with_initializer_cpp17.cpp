// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : if_switch_with_initializer_cpp17
// Topic id : part2/stage08/section02/if_switch_with_initializer_cpp17
//
// Covers: if/switch with init-statement (C++17)

#include "learn/topic_registry.hpp"

#include <map>
#include <string>

namespace {

int lookup(const std::map<std::string, int>& m, const std::string& key) {
    if (auto it = m.find(key); it != m.end()) {
        return it->second;
    }
    return -1;
}

const char* grade(int score) {
    switch (int g = score / 10; g) {
        case 10:
        case 9:
            return "A";
        case 8:
            return "B";
        case 7:
            return "C";
        default:
            return "D";
    }
}

void demo_basics() {
    std::map<std::string, int> m{{"a", 1}, {"b", 2}};
    LEARN_CHECK(lookup(m, "a") == 1);
    LEARN_CHECK(lookup(m, "z") == -1);
}

void demo_intermediate() {
    LEARN_CHECK(std::string{grade(95)} == "A");
    LEARN_CHECK(std::string{grade(85)} == "B");
    LEARN_CHECK(std::string{grade(50)} == "D");
}

void demo_expert() {
    // Init variable scope limited to if/else.
    int out = 0;
    if (int x = 10; x > 5) {
        out = x * 2;
    } else {
        out = -1;
    }
    LEARN_CHECK(out == 20);

    // x is not in scope here — use out only.
    LEARN_CHECK(out != 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/if_switch_with_initializer_cpp17", run>;

}  // namespace
