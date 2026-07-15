// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : null_pointer_dereference
// Topic id : part2/stage09/section08/null_pointer_dereference
//
// Covers: null deref is UB — only show checked/safe paths

#include "learn/topic_registry.hpp"

#include <cassert>
#include <optional>

namespace {

int read_if(const int* p) {
    // SAFE: check before use. Never: return *p without check when p may be null.
    if (p == nullptr) {
        return -1;
    }
    return *p;
}

std::optional<int> try_read(const int* p) {
    if (!p) {
        return std::nullopt;
    }
    return *p;
}

void demo_basics() {
    int x = 7;
    assert(read_if(&x) == 7);
    assert(read_if(nullptr) == -1);
}

void demo_intermediate() {
    int y = 3;
    assert(try_read(&y).value() == 3);
    assert(!try_read(nullptr).has_value());
}

void demo_expert() {
    // Prefer references when null is not allowed.
    auto need_ref = [](const int& r) { return r + 1; };
    int z = 41;
    assert(need_ref(z) == 42);

    // Comment only: `int* p = nullptr; int v = *p;` is UB — never do this.
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/null_pointer_dereference", run>;

}  // namespace
