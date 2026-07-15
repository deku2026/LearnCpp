// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : profilers_overview
// Topic id : part4/section05/profilers_overview
//
// Covers: profilers: perf, VTune, Visual Studio, gprof

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Measure before optimizing
    std::string motto = "profile first";
    assert(!motto.empty());
}

void demo_intermediate() {
    // Sampling vs instrumentation profilers
    assert(true);
}

void demo_expert() {
    // Symbols required (RelWithDebInfo)
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/profilers_overview", run>;

}  // namespace
