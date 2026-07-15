// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section03_consteval_constinit
// Item     : constinit_static_init_order
// Topic id : part2/stage07/section03/constinit_static_init_order
//
// Covers: constinit guarantees constant initialization (no dynamic init order fiasco)

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

constexpr int compute() {
    return 40 + 2;
}

// constinit: must be constant-initialized; can still be mutated later if not const.
constinit int g_counter = compute();

constinit const int g_fixed = compute();

struct Holder {
    static constinit int value;
};

constinit int Holder::value = 100;

void demo_basics() {
    assert(g_counter == 42);
    assert(g_fixed == 42);
}

void demo_intermediate() {
    g_counter += 1;
    assert(g_counter == 43);
    g_counter = 42;         // restore for other tests if re-run
    assert(g_fixed == 42);  // const object
}

void demo_expert() {
    assert(Holder::value == 100);
    Holder::value = 101;
    assert(Holder::value == 101);
    Holder::value = 100;

    // Teaching: constinit prevents dynamic initialization; static init order
    // across TUs for dynamic init is the classic fiasco constinit helps avoid.
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section03/constinit_static_init_order", run>;

}  // namespace
