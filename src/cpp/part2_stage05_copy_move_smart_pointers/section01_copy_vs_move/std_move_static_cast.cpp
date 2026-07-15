// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : std_move_static_cast
// Topic id : part2/stage05/section01/std_move_static_cast
//
// Covers: std::move is only a cast to rvalue; named rvalue refs are lvalues

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>

namespace {

template <class T>
constexpr std::remove_reference_t<T>&& my_move(T&& x) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(x);
}

struct Tracker {
    int copies = 0;
    int moves = 0;

    Tracker() = default;
    Tracker(const Tracker& o) : copies(o.copies + 1), moves(o.moves) {}
    Tracker(Tracker&& o) noexcept : copies(o.copies), moves(o.moves + 1) {}
};

struct Wrapper {
    std::string s;
    Wrapper() = default;
    Wrapper(Wrapper&& other) noexcept : s(std::move(other.s)) {}  // named && is lvalue
};

void demo_basics() {
    std::string a = "hello";
    std::string b = std::move(a);  // cast enables move ctor selection
    assert(b == "hello");

    const std::string c = "world";
    std::string d = std::move(c);  // const rvalue -> copy, not move
    assert(d == "world");
    assert(c == "world");
}

void demo_intermediate() {
    Tracker t;
    Tracker u = my_move(t);
    assert(u.moves == 1);
    assert(u.copies == 0);

    int x = 7;
    int y = std::move(x);  // trivial types: move == copy
    assert(y == 7);
    assert(x == 7);
}

void demo_expert() {
    static_assert(std::is_same_v<decltype(std::move(std::declval<std::string&>())), std::string&&>);
    static_assert(std::is_same_v<decltype(std::move(std::declval<const std::string&>())), const std::string&&>);

    Wrapper w;
    w.s = "payload";
    Wrapper w2 = std::move(w);
    assert(w2.s == "payload");

    // Named rvalue-reference parameter is an lvalue expression.
    auto take = [](std::string&& rr) {
        std::string local = std::move(rr);  // need move again
        return local;
    };
    std::string out = take(std::string{"x"});
    assert(out == "x");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/std_move_static_cast", run>;

}  // namespace
