// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section03_copy_elision
// Item     : never_move_return_of_local
// Topic id : part2/stage05/section03/never_move_return_of_local
//
// Covers: F.48 — do not return std::move(local); it blocks NRVO

#include "learn/topic_registry.hpp"

#include <string>
#include <utility>

namespace {

struct Widget {
    static int moves;
    std::string s;

    Widget() = default;
    explicit Widget(std::string v) : s(std::move(v)) {}
    Widget(const Widget& o) : s(o.s) {}
    Widget(Widget&& o) noexcept : s(std::move(o.s)) { ++moves; }
};
int Widget::moves = 0;

Widget make_good() {
    Widget w("good");
    return w;  // NRVO or implicit move
}

Widget make_pessimizing() {
    Widget w("bad");
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpessimizing-move"
#endif
    return std::move(w);  // intentional anti-pattern: prevents NRVO; forces a move
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
}

void demo_basics() {
    Widget::moves = 0;
    Widget a = make_good();
    LEARN_CHECK(a.s == "good");
    // Good path: 0 (NRVO) or 1 (implicit move) moves — never a required copy.
    LEARN_CHECK(Widget::moves <= 1);
}

void demo_intermediate() {
    Widget::moves = 0;
    Widget b = make_pessimizing();
    LEARN_CHECK(b.s == "bad");
    // Pessimizing move: at least one move (NRVO suppressed).
    LEARN_CHECK(Widget::moves >= 1);
}

void demo_expert() {
    // When returning a member (not the local object itself), move is correct.
    auto member = []() -> std::string {
        Widget w("member");
        return std::move(w.s);
    };
    LEARN_CHECK(member() == "member");

    // Returning a function parameter by value: move is appropriate.
    auto from_param = [](std::string s) -> std::string {
        return s;  // implicit move from by-value param
    };
    LEARN_CHECK(from_param("param") == "param");

    auto good_string = []() -> std::string {
        std::string local = "plain";
        return local;  // never std::move(local)
    };
    LEARN_CHECK(good_string() == "plain");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section03/never_move_return_of_local", run>;

}  // namespace
