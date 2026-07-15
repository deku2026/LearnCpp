// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : simplified_implicit_move_cpp23
// Topic id : part2/stage05/section01/simplified_implicit_move_cpp23
//
// Covers: C++23 P2266 simplified implicit move; return local prefers move

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <utility>

namespace {

struct Widget {
    std::string s;
    int moves = 0;
    int copies = 0;

    Widget() = default;
    explicit Widget(std::string v) : s(std::move(v)) {}
    Widget(const Widget& o) : s(o.s), moves(o.moves), copies(o.copies + 1) {}
    Widget(Widget&& o) noexcept : s(std::move(o.s)), moves(o.moves + 1), copies(o.copies) {}
};

Widget make_by_value() {
    Widget w("local");
    return w;  // implicit move (or NRVO); never needs std::move
}

// C++23: returning rvalue-ref from move-eligible id is xvalue (P2266).
Widget&& passthrough(Widget&& w) {
    return std::move(w);  // portable form; C++23 also allows `return w;`
}

void demo_basics() {
    Widget w = make_by_value();
    assert(w.s == "local");
    // Either NRVO (0 moves) or implicit move (>=1); never a required copy path.
    assert(w.copies == 0);
}

void demo_intermediate() {
    Widget src("data");
    Widget&& rr = passthrough(std::move(src));
    Widget dst = std::move(rr);
    assert(dst.s == "data");
    assert(dst.moves >= 1);
}

void demo_expert() {
    // Returning a local by value: prefer bare name (implicit move / NRVO).
    auto factory = []() -> std::string {
        std::string local = "abc";
        return local;
    };
    assert(factory() == "abc");

    // When returning a member of a local, move is intentional (no NRVO on member).
    auto member_out = []() -> std::string {
        Widget w("member");
        return std::move(w.s);
    };
    assert(member_out() == "member");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/simplified_implicit_move_cpp23", run>;

}  // namespace
