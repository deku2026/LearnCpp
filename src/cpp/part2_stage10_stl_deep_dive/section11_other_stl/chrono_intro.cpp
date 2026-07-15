// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : chrono_intro
// Topic id : part2/stage10/section11_other_stl/chrono_intro
//
// Covers: chrono duration clock time_point basics

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <thread>

namespace {

void demo_basics() {
    using namespace std::chrono_literals;
    auto d = 100ms;
    assert(d == std::chrono::milliseconds(100));
    assert(std::chrono::duration_cast<std::chrono::seconds>(1500ms).count() == 1);
}

void demo_intermediate() {
    const auto t0 = std::chrono::steady_clock::now();
    const auto t1 = t0 + std::chrono::milliseconds(50);
    assert(t1 > t0);
    assert(std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() == 50);
}

void demo_expert() {
    using clock = std::chrono::system_clock;
    const clock::time_point tp = clock::now();
    const std::time_t tt = clock::to_time_t(tp);
    assert(tt != 0 || tt == 0);
    auto sec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
    assert(sec.count() > 0);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11_other_stl/chrono_intro", run>;

}  // namespace
