// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : noexcept_move_and_vector_growth
// Topic id : part2/stage05/section01/noexcept_move_and_vector_growth
//
// Covers: noexcept move enables vector reallocation by move, not copy

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

struct Movable {
    static int copies;
    static int moves;
    int id = 0;

    Movable() = default;
    explicit Movable(int i) : id(i) {}
    Movable(const Movable& o) : id(o.id) { ++copies; }
    Movable(Movable&& o) noexcept : id(o.id) { ++moves; }
    Movable& operator=(const Movable& o) {
        id = o.id;
        ++copies;
        return *this;
    }
    Movable& operator=(Movable&& o) noexcept {
        id = o.id;
        ++moves;
        return *this;
    }
};
int Movable::copies = 0;
int Movable::moves = 0;

struct ThrowMovable {
    static int copies;
    static int moves;
    int id = 0;

    ThrowMovable() = default;
    explicit ThrowMovable(int i) : id(i) {}
    ThrowMovable(const ThrowMovable& o) : id(o.id) { ++copies; }
    ThrowMovable(ThrowMovable&& o) : id(o.id) { ++moves; }  // not noexcept
    ThrowMovable& operator=(const ThrowMovable& o) {
        id = o.id;
        ++copies;
        return *this;
    }
    ThrowMovable& operator=(ThrowMovable&& o) {
        id = o.id;
        ++moves;
        return *this;
    }
};
int ThrowMovable::copies = 0;
int ThrowMovable::moves = 0;

void demo_basics() {
    static_assert(std::is_nothrow_move_constructible_v<Movable>);
    static_assert(!std::is_nothrow_move_constructible_v<ThrowMovable>);
    static_assert(std::is_nothrow_move_constructible_v<std::vector<int>>);
}

void demo_intermediate() {
    Movable::copies = 0;
    Movable::moves = 0;

    std::vector<Movable> v;
    v.reserve(1);
    v.emplace_back(1);
    v.emplace_back(2);  // reallocation: should move if noexcept

    assert(v.size() == 2);
    assert(Movable::moves >= 1);
    assert(Movable::copies == 0);
}

void demo_expert() {
    ThrowMovable::copies = 0;
    ThrowMovable::moves = 0;

    std::vector<ThrowMovable> v;
    v.reserve(1);
    v.emplace_back(1);
    v.emplace_back(2);  // reallocation: prefers copy for strong exception guarantee

    assert(v.size() == 2);
    assert(ThrowMovable::copies >= 1);

    // move_if_noexcept selects copy for potentially-throwing move.
    ThrowMovable src(9);
    ThrowMovable::copies = 0;
    ThrowMovable::moves = 0;
    ThrowMovable dst(std::move_if_noexcept(src));
    assert(ThrowMovable::copies == 1);
    assert(ThrowMovable::moves == 0);
    assert(dst.id == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/noexcept_move_and_vector_growth", run>;

}  // namespace
