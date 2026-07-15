// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : tsan_overview
// Topic id : part4/section05/tsan_overview
//
// Covers: ThreadSanitizer data race detection

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // -fsanitize=thread
    // Safe: mutex-protected or atomic
    std::string note = "join all threads";
    assert(!note.empty());
}

void demo_intermediate() {
    // Separate CI lane from ASan
    assert(true);
}

void demo_expert() {
    // Higher memory overhead
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/tsan_overview", run>;

}  // namespace
