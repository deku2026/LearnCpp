// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_static_polymorphism
// Topic id : part2/stage06/section06/crtp_static_polymorphism
//
// Covers: CRTP for static polymorphism without virtual

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

template <typename Derived>
struct Shape {
    int area() const { return static_cast<const Derived*>(this)->area_impl(); }
    std::string name() const { return static_cast<const Derived*>(this)->name_impl(); }
};

struct Square : Shape<Square> {
    int side = 0;
    int area_impl() const { return side * side; }
    std::string name_impl() const { return "square"; }
};

struct Rect : Shape<Rect> {
    int w = 0;
    int h = 0;
    int area_impl() const { return w * h; }
    std::string name_impl() const { return "rect"; }
};

template <typename D>
int double_area(const Shape<D>& s) {
    return s.area() * 2;
}

void demo_basics() {
    Square sq;
    sq.side = 4;
    assert(sq.area() == 16);
    assert(sq.name() == "square");
}

void demo_intermediate() {
    Rect r;
    r.w = 3;
    r.h = 5;
    assert(r.area() == 15);
    assert(double_area(r) == 30);
}

void demo_expert() {
    // No vtable: different Derived => different monomorphs of Shape/double_area.
    Square sq;
    sq.side = 2;
    assert(double_area(sq) == 8);
    assert(sq.name() != Rect{}.name());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_static_polymorphism", run>;

}  // namespace
