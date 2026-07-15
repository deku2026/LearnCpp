// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section02_noexcept_basics
// Item     : move_should_be_noexcept_for_vector_growth
// Topic id : part2/stage09/section02/move_should_be_noexcept_for_vector_growth
//
// Covers: vector prefers noexcept moves when reallocating

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <utility>
#include <vector>

namespace {

struct NoexceptMovable {
    int v = 0;
    static int moves;
    NoexceptMovable() = default;
    explicit NoexceptMovable(int x) : v(x) {}
    NoexceptMovable(const NoexceptMovable& o) : v(o.v) {}
    NoexceptMovable(NoexceptMovable&& o) noexcept : v(o.v) {
        ++moves;
        o.v = 0;
    }
    NoexceptMovable& operator=(NoexceptMovable&& o) noexcept {
        v = o.v;
        o.v = 0;
        ++moves;
        return *this;
    }
};
int NoexceptMovable::moves = 0;

struct ThrowingMove {
    int v = 0;
    ThrowingMove() = default;
    explicit ThrowingMove(int x) : v(x) {}
    ThrowingMove(const ThrowingMove& o) : v(o.v) {}
    ThrowingMove(ThrowingMove&& o) noexcept(false) : v(o.v) { o.v = 0; }
    ThrowingMove& operator=(ThrowingMove&& o) noexcept(false) {
        v = o.v;
        o.v = 0;
        return *this;
    }
};

void demo_basics() {
    static_assert(std::is_nothrow_move_constructible_v<NoexceptMovable>);
    static_assert(!std::is_nothrow_move_constructible_v<ThrowingMove>);
}

void demo_intermediate() {
    NoexceptMovable::moves = 0;
    std::vector<NoexceptMovable> v;
    v.reserve(1);
    v.emplace_back(1);
    v.emplace_back(2);  // may reallocate and move
    LEARN_CHECK(v.size() == 2);
    LEARN_CHECK(v[0].v == 1);
    LEARN_CHECK(NoexceptMovable::moves >= 0);
}

void demo_expert() {
    // Throwing move => vector may copy on reallocation instead of moving.
    std::vector<ThrowingMove> v;
    v.emplace_back(1);
    v.emplace_back(2);
    LEARN_CHECK(v.size() == 2);
    LEARN_CHECK(v[1].v == 2);
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
    ::learn::topic<"part2/stage09/section02/move_should_be_noexcept_for_vector_growth", run>;

}  // namespace
