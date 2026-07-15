// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : suppression_rules
// Topic id : part2/stage04/section03/suppression_rules
//
// Covers: declaring dtor/copy suppresses move; declaring move deletes copy

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <utility>

namespace {

struct WithDtor {
    int x = 0;
    ~WithDtor() {}  // user-declared dtor suppresses implicit move
};

struct WithMove {
    int x = 0;
    WithMove() = default;
    WithMove(WithMove&&) = default;
    WithMove& operator=(WithMove&&) = default;
};

struct Restored {
    int x = 0;
    Restored() = default;
    Restored(const Restored&) = default;
    Restored& operator=(const Restored&) = default;
    Restored(Restored&&) = default;
    Restored& operator=(Restored&&) = default;
    ~Restored() = default;
};

void demo_basics() {
    // User dtor => not move constructible as a true move (may still copy).
    static_assert(std::is_copy_constructible_v<WithDtor>);
    // Implementation may still report move constructible via copy fallback.
    WithDtor a;
    a.x = 3;
    WithDtor b = std::move(a);
    LEARN_CHECK(b.x == 3);
    // After "move" that fell back to copy, source is unchanged.
    LEARN_CHECK(a.x == 3);
}

void demo_intermediate() {
    static_assert(std::is_move_constructible_v<WithMove>);
    static_assert(!std::is_copy_constructible_v<WithMove>);

    WithMove m;
    m.x = 9;
    WithMove n = std::move(m);
    LEARN_CHECK(n.x == 9);
}

void demo_expert() {
    // If you declare any of dtor/copy/move, define or delete the full set.
    static_assert(std::is_copy_constructible_v<Restored>);
    static_assert(std::is_move_constructible_v<Restored>);
    Restored r;
    r.x = 1;
    Restored s = std::move(r);
    LEARN_CHECK(s.x == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/suppression_rules", run>;

}  // namespace
