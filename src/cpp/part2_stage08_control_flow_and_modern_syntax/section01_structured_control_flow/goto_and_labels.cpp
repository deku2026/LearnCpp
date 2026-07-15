// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : goto_and_labels
// Topic id : part2/stage08/section01/goto_and_labels
//
// Covers: goto for multi-level exit (prefer structured control when possible)

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>

namespace {

int find_in_matrix(const std::array<std::array<int, 3>, 3>& m, int target) {
    int pos = -1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (m[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] == target) {
                pos = i * 10 + j;
                goto found;
            }
        }
    }
found:
    return pos;
}

int skip_with_goto(int n) {
    int s = 0;
    int i = 0;
loop:
    if (i >= n) {
        goto done;
    }
    s += i;
    ++i;
    goto loop;
done:
    return s;
}

void demo_basics() {
    std::array<std::array<int, 3>, 3> m{{{{1, 2, 3}}, {{4, 5, 6}}, {{7, 8, 9}}}};
    assert(find_in_matrix(m, 5) == 11);
    assert(find_in_matrix(m, 9) == 22);
}

void demo_intermediate() {
    assert((find_in_matrix({{{{0, 0, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}}, 1) == -1));
    assert(skip_with_goto(5) == 10);
}

void demo_expert() {
    // Prefer break/return; goto is shown for multi-level exit teaching only.
    int hits = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ++hits;
            if (hits == 4) {
                goto out;
            }
        }
    }
out:
    assert(hits == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/goto_and_labels", run>;

}  // namespace
