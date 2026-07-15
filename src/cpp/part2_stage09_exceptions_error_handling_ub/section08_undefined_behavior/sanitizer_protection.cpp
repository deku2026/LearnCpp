// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : sanitizer_protection
// Topic id : part2/stage09/section08/sanitizer_protection
//
// Covers: ASan/UBSan mindset — write checkable safe code; don't trigger bugs

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace {

// Patterns that *would* be caught by sanitizers if written incorrectly:
// - heap-buffer-overflow, use-after-free (ASan)
// - signed-integer-overflow, null deref (UBSan)
// We only show the fixed versions.

int sum_checked(const std::vector<int>& v) {
    int s = 0;
    for (std::size_t i = 0; i < v.size(); ++i) {
        s += v[i];
    }
    return s;
}

void demo_basics() {
    std::vector<int> v{1, 2, 3};
    assert(sum_checked(v) == 6);
}

void demo_intermediate() {
    auto p = std::make_unique<int[]>(4);
    for (int i = 0; i < 4; ++i) {
        p[static_cast<std::size_t>(i)] = i;
    }
    assert(p[0] == 0 && p[3] == 3);
    // Comment: p[4] would be ASan heap-buffer-overflow — not done.
}

void demo_expert() {
    // Defensive coding that keeps CI green under -fsanitize=address,undefined
    int a = 100;
    int b = 200;
    // SAFE add with wider type if needed
    long long s = static_cast<long long>(a) + b;
    assert(s == 300);

    int* p = nullptr;
    if (p != nullptr) {
        assert(*p == 0);
    }
    assert(p == nullptr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/sanitizer_protection", run>;

}  // namespace
