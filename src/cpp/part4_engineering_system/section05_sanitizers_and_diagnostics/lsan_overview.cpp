// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : lsan_overview
// Topic id : part4/section05/lsan_overview
//
// Covers: LeakSanitizer

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Often bundled with ASan; ASAN_OPTIONS=detect_leaks=1
    auto p = std::string{"owned"};
    assert(p == "owned");
}

void demo_intermediate() {
    // Prefer RAII to avoid leaks
    assert(true);
}

void demo_expert() {
    // Suppressions for third-party only when necessary
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/lsan_overview", run>;

}  // namespace
