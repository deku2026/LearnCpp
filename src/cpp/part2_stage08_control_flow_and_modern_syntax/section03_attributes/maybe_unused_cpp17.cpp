// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : maybe_unused_cpp17
// Topic id : part2/stage08/section03/maybe_unused_cpp17
//
// Covers: [[maybe_unused]] for parameters, locals, statics

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

int add(int a, int b, [[maybe_unused]] int debug_tag) {
    return a + b;
}

void demo_basics() {
    assert(add(2, 3, 99) == 5);
}

void demo_intermediate() {
    [[maybe_unused]] int only_for_debug = 1;
    int x = 10;
    assert(x == 10);
}

void demo_expert() {
    enum class Mode { A, B };
    [[maybe_unused]] Mode m = Mode::A;

    auto f = [](int v, [[maybe_unused]] const char* name) { return v * 2; };
    assert(f(21, "answer") == 42);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/maybe_unused_cpp17", run>;

}  // namespace
