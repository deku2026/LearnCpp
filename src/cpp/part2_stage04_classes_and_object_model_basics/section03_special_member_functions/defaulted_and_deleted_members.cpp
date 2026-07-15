// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : defaulted_and_deleted_members
// Topic id : part2/stage04/section03/defaulted_and_deleted_members
//
// Covers: =default vs =delete for special members and selected overloads

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <utility>

namespace {

struct MovableOnly {
    int v = 0;
    MovableOnly() = default;
    MovableOnly(const MovableOnly&) = delete;
    MovableOnly& operator=(const MovableOnly&) = delete;
    MovableOnly(MovableOnly&&) = default;
    MovableOnly& operator=(MovableOnly&&) = default;
    ~MovableOnly() = default;
};

struct NoDouble {
    void take(int) {}
    void take(double) = delete;
};

void demo_basics() {
    MovableOnly a;
    a.v = 3;
    MovableOnly b = std::move(a);
    LEARN_CHECK(b.v == 3);

    static_assert(!std::is_copy_constructible_v<MovableOnly>);
    static_assert(std::is_move_constructible_v<MovableOnly>);
}

void demo_intermediate() {
    NoDouble n;
    n.take(1);
    // n.take(1.0); // would be ill-formed (deleted overload)
    LEARN_CHECK(true);
}

void demo_expert() {
    // =default keeps triviality when possible; hand-written empty body may not.
    struct Trivial {
        int x = 0;
        Trivial() = default;
        Trivial(const Trivial&) = default;
        Trivial& operator=(const Trivial&) = default;
        ~Trivial() = default;
    };
    static_assert(std::is_trivially_copyable_v<Trivial>);
    Trivial t{};
    Trivial u = t;
    LEARN_CHECK(u.x == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/defaulted_and_deleted_members", run>;

}  // namespace
