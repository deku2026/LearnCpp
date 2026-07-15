// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : init_statement_alias_declaration_cpp23
// Topic id : part2/stage08/section04/init_statement_alias_declaration_cpp23
//
// Covers: alias declaration in if/for init (C++23) + portable using outside

#include "learn/topic_registry.hpp"

#include <string>
#include <vector>

namespace {

void demo_basics() {
    using Vec = std::vector<int>;
    Vec v{1, 2, 3};
    int s = 0;
    for (int x : v) {
        s += x;
    }
    LEARN_CHECK(s == 6);
}

void demo_intermediate() {
    // Portable: alias + if with initializer (C++17 if-init).
    using Str = std::string;
    if (Str s = "hello"; !s.empty()) {
        LEARN_CHECK(s.size() == 5);
    }
}

void demo_expert() {
    // C++23 allows alias-declaration as the init-statement of if/for.
    // Portable teaching form keeps `using` in the enclosing block.
    using IVec = std::vector<int>;
    int seen = 0;
    for (IVec tmp{10, 20}; int x : tmp) {
        seen += x;
    }
    LEARN_CHECK(seen == 30);

    using T = int;
    T a = 21;
    T b = 21;
    LEARN_CHECK(a + b == 42);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section04/init_statement_alias_declaration_cpp23", run>;

}  // namespace
