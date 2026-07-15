// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : asan_overview
// Topic id : part4/section05/asan_overview
//
// Covers: AddressSanitizer catches heap/stack buffer issues

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // -fsanitize=address /fsanitize=address
    // Demo safe code only — do not intentionally overflow
    int a[3] = {1, 2, 3};
    assert(a[0] == 1 && a[2] == 3);
}

void demo_intermediate() {
    // Detects use-after-free, OOB, some leaks (with LSan)
    assert(true);
}

void demo_expert() {
    // Not compatible with TSan in same build typically
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/asan_overview", run>;

}  // namespace
