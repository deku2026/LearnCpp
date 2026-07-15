// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : pvs_studio_intro
// Topic id : part4/section03/pvs_studio_intro
//
// Covers: PVS-Studio commercial analyzer intro

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Commercial static analyzer; free tiers for open source/students
    std::string tool = "PVS-Studio";
    assert(!tool.empty());
}

void demo_intermediate() {
    // Complements clang-tidy/cppcheck
    assert(true);
}

void demo_expert() {
    // Integrate incremental analysis in CI when licensed
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/pvs_studio_intro", run>;

}  // namespace
