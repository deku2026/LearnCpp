// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : non_copyable_non_movable_via_delete
// Topic id : part2/stage15/section04/non_copyable_non_movable_via_delete
//
// Covers: =delete for non-copyable/movable

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

struct Token {
    int id;
    explicit Token(int i) : id(i) {}
    Token(const Token&) = delete;
    Token& operator=(const Token&) = delete;
    Token(Token&&) = delete;
    Token& operator=(Token&&) = delete;
};

void demo_basics() {
    Token t(1);
    LEARN_CHECK(t.id == 1);
    static_assert(!std::is_copy_constructible_v<Token>);
}

void demo_intermediate() {
    static_assert(!std::is_move_constructible_v<Token>);
    static_assert(!std::is_copy_assignable_v<Token>);
}

void demo_expert() {
    // Unique ownership / identity types often delete copy and move.
    Token a(3);
    Token b(4);
    LEARN_CHECK(a.id != b.id);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section04/non_copyable_non_movable_via_delete", run>;

}  // namespace
