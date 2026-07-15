// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section02_noexcept_basics
// Item     : noexcept_specifier
// Topic id : part2/stage09/section02/noexcept_specifier
//
// Covers: noexcept / noexcept(bool) specifier on functions

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>
#include <utility>

namespace {

void clean() noexcept {}

void maybe(bool b) noexcept(false) {
    if (b) {
        // no throw in this demo path
    }
}

template <typename T>
void swap_like(T& a, T& b) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

void demo_basics() {
    clean();
    maybe(false);
    static_assert(noexcept(clean()));
}

void demo_intermediate() {
    int a = 1;
    int b = 2;
    swap_like(a, b);
    assert(a == 2 && b == 1);
    static_assert(noexcept(swap_like(a, b)));
}

void demo_expert() {
    struct NoexceptMove {
        NoexceptMove() = default;
        NoexceptMove(NoexceptMove&&) noexcept = default;
        NoexceptMove& operator=(NoexceptMove&&) noexcept = default;
    };
    NoexceptMove x, y;
    swap_like(x, y);
    static_assert(std::is_nothrow_move_constructible_v<NoexceptMove>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section02/noexcept_specifier", run>;

}  // namespace
