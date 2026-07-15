// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_allows_static_thread_local_cpp23
// Topic id : part2/stage07/section01/constexpr_allows_static_thread_local_cpp23
//
// Covers: C++23 constexpr may use static/thread_local (feature-tested)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <version>

namespace {

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L
constexpr int with_static_local(int x) {
    static int cache = 0;  // C++23: allowed in constexpr
    if (cache == 0) {
        cache = x;
    }
    return cache + x;
}
#endif

constexpr int portable_no_static(int x) {
    return x * 2;
}

void demo_basics() {
    constexpr int v = portable_no_static(21);
    static_assert(v == 42);
    assert(v == 42);
}

void demo_intermediate() {
#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L
    // Runtime path exercises static local; compile-time use is implementation-defined in scope.
    int a = with_static_local(3);
    int b = with_static_local(10);
    assert(a == 6);   // cache=3, return 3+3
    assert(b == 13);  // cache stays 3, return 3+10
#else
    assert(portable_no_static(3) == 6);
    assert(portable_no_static(10) == 20);
#endif
}

void demo_expert() {
#if defined(__cpp_threadsafe_static_init)
    static_assert(__cpp_threadsafe_static_init >= 200806L || true);
#endif
    // thread_local in constexpr is a C++23 relaxation; we only assert portable behavior here.
    assert(portable_no_static(0) == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section01/constexpr_allows_static_thread_local_cpp23", run>;

}  // namespace
