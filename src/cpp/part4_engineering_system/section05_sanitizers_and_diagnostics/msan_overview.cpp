// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : msan_overview
// Topic id : part4/section05/msan_overview
//
// Covers: MemorySanitizer uninitialized reads (Clang)

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // -fsanitize=memory (Clang); requires instrumented libs
    int x = 0;
    LEARN_CHECK(x == 0);
}

void demo_intermediate() {
    // Not for MSVC; Linux Clang primary
    LEARN_CHECK(true);
}

void demo_expert() {
    // Cannot mix with ASan
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/msan_overview", run>;

}  // namespace
