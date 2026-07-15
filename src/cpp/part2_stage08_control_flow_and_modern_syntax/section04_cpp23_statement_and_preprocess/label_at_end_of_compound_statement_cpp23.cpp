// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : label_at_end_of_compound_statement_cpp23
// Topic id : part2/stage08/section04/label_at_end_of_compound_statement_cpp23
//
// Covers: C++23 allows labels at end of compound statement

#include "learn/topic_registry.hpp"

namespace {

int with_end_label(int x) {
    int r = x;
    if (x < 0) {
        r = -x;
        goto done;
    }
    r = x * 2;
done:
    // C++23: label may appear immediately before closing brace.
    return r;
}

int loop_end_label(int n) {
    int s = 0;
    int i = 0;
    {
    again:
        if (i >= n) {
            goto finish;
        }
        s += i;
        ++i;
        goto again;
    finish:;
    }
    return s;
}

void demo_basics() {
    LEARN_CHECK(with_end_label(5) == 10);
    LEARN_CHECK(with_end_label(-3) == 3);
}

void demo_intermediate() {
    LEARN_CHECK(loop_end_label(5) == 10);
    LEARN_CHECK(loop_end_label(0) == 0);
}

void demo_expert() {
    // Pre-C++23 required a null statement after a terminal label in some cases.
    // Portable pattern: `finish:;` still works everywhere.
    int x = 1;
    goto end;
    x = 0;
end:;
    LEARN_CHECK(x == 1);
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
    ::learn::topic<"part2/stage08/section04/label_at_end_of_compound_statement_cpp23", run>;

}  // namespace
