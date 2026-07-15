// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : zero_initialization_static_thread_local
// Topic id : part2/stage02/section02/zero_initialization_static_thread_local
//
// Covers: static/thread_local zero-init before dynamic init

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>

namespace {

int g_plain;          // zero-initialized
static int g_static;  // zero-initialized
int g_array[3];       // each element zero-initialized

struct Pod {
    int x;
    double y;
};
Pod g_pod;  // zero-initialized members

thread_local int tls_counter;  // zero-initialized per thread

void demo_basics() {
    assert(g_plain == 0);
    assert(g_static == 0);
    assert(g_array[0] == 0 && g_array[1] == 0 && g_array[2] == 0);
    assert(g_pod.x == 0 && g_pod.y == 0.0);
}

void demo_intermediate() {
    // Function-local static is zero-init then dynamic-init once.
    static int local_static;
    assert(local_static == 0);
    local_static = 42;
    assert(local_static == 42);

    // Second call path still sees the updated value (not re-zeroed).
    static int once = 0;
    ++once;
    assert(once >= 1);

    assert(tls_counter == 0);
    tls_counter = 7;
    assert(tls_counter == 7);
}

void demo_expert() {
    // Pointer and nullptr-sized static storage
    static int* sp;
    static void* vp;
    assert(sp == nullptr);
    assert(vp == nullptr);

    static std::int64_t wide;
    assert(wide == 0);

    // Constexpr static storage is compile-time; still conceptually zero or constant.
    static constexpr int k = 0;
    assert(k == 0);

    // Zero-init happens before any dynamic initializer runs.
    static int ordered = g_plain + 1;  // g_plain already 0
    assert(ordered == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/zero_initialization_static_thread_local", run>;

}  // namespace
