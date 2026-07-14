// LearnCpp placeholder
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : profilers_overview
// Topic id : part4/section05/profilers_overview
//
// TODO: read cppreference, sketch a minimal example, check godbolt / C++ Insights,
//       then replace this empty run() body with real demo code.

#include "learn/topic_registry.hpp"

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/profilers_overview", run>;

}  // namespace
