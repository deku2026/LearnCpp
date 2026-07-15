// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : rvo
// Topic id : part2/stage05/section03/rvo
//
// Covers: RVO / URVO — return prvalue constructed in caller slot

#include "learn/topic_registry.hpp"

#include <string>

namespace {

struct Noisy {
    static int ctors;
    static int copies;
    static int moves;

    int value = 0;

    Noisy() { ++ctors; }
    explicit Noisy(int v) : value(v) { ++ctors; }
    Noisy(const Noisy& o) : value(o.value) { ++copies; }
    Noisy(Noisy&& o) noexcept : value(o.value) { ++moves; }
};
int Noisy::ctors = 0;
int Noisy::copies = 0;
int Noisy::moves = 0;

Noisy makeRVO() {
    return Noisy(42);
}  // prvalue return

void demo_basics() {
    Noisy::ctors = Noisy::copies = Noisy::moves = 0;
    Noisy a = makeRVO();
    LEARN_CHECK(a.value == 42);
    // C++17 guaranteed elision for prvalue: no copy/move required.
    LEARN_CHECK(Noisy::ctors == 1);
    LEARN_CHECK(Noisy::copies == 0);
    LEARN_CHECK(Noisy::moves == 0);
}

void demo_intermediate() {
    auto factory = []() -> std::string { return std::string("rvo"); };
    std::string s = factory();
    LEARN_CHECK(s == "rvo");
}

void demo_expert() {
    // Even with deleted copy/move, prvalue initialization is valid (C++17).
    struct OnlyPrvalue {
        int x;
        OnlyPrvalue(int v) : x(v) {}
        OnlyPrvalue(const OnlyPrvalue&) = delete;
        OnlyPrvalue(OnlyPrvalue&&) = delete;
        OnlyPrvalue& operator=(const OnlyPrvalue&) = delete;
        OnlyPrvalue& operator=(OnlyPrvalue&&) = delete;
    };
    auto make = []() -> OnlyPrvalue { return OnlyPrvalue{7}; };
    OnlyPrvalue o = make();
    LEARN_CHECK(o.x == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section03/rvo", run>;

}  // namespace
