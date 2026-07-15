// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : vector_bool_pitfall
// Topic id : part2/stage10/section01_sequence_containers/vector_bool_pitfall
//
// Covers: vector<bool> proxy reference, not a real container of bool

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <deque>
#include <vector>

namespace {

void demo_basics() {
    std::vector<bool> vb{true, false, true};
    assert(vb.size() == 3);
    assert(vb[0]);
    assert(!vb[1]);
    // vb[i] is a proxy, not bool&
    auto proxy = vb[0];
    assert(static_cast<bool>(proxy));
}

void demo_intermediate() {
    std::vector<bool> vb(4, false);
    vb[2] = true;
    assert(vb[2]);
    // Prefer real bool storage when references/pointers are needed
    std::vector<char> flags{0, 1, 0, 1};
    char& r = flags[1];
    r = 0;
    assert(flags[1] == 0);
}

void demo_expert() {
    std::array<bool, 3> ab{true, false, true};
    bool& ref = ab[0];
    ref = false;
    assert(!ab[0]);

    std::deque<bool> db{true, false};
    bool& dref = db[0];
    dref = false;
    assert(!db[0]);

    std::vector<bool> vb{true};
    // cannot form bool* to element; use alternatives above for addressability
    assert(vb.front());
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section01_sequence_containers/vector_bool_pitfall", run>;

}  // namespace
