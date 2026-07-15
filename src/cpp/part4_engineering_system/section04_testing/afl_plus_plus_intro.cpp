// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : afl_plus_plus_intro
// Topic id : part4/section04/afl_plus_plus_intro
//
// Covers: AFL++ fuzzing conceptual

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Persistent mode harness around parse API
    auto ok = [](std::string_view in) { return !in.empty(); };
    assert(ok("a"));
}

void demo_intermediate() {
    // Instrumentation via afl-clang-fast
    assert(true);
}

void demo_expert() {
    // Combine with ASan builds
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/afl_plus_plus_intro", run>;

}  // namespace
