// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_inject_behavior
// Topic id : part2/stage06/section06/crtp_inject_behavior
//
// Covers: CRTP mixins injecting equality / counting behavior

#include "learn/topic_registry.hpp"

#include <string>

namespace {

template <typename Derived>
struct EqualityMixin {
    friend bool operator==(const Derived& a, const Derived& b) { return a.equal_to(b); }
    friend bool operator!=(const Derived& a, const Derived& b) { return !(a == b); }
};

template <typename Derived>
struct Counted {
    static int live;
    Counted() { ++live; }
    Counted(const Counted&) { ++live; }
    ~Counted() { --live; }
};

template <typename Derived>
int Counted<Derived>::live = 0;

struct Point : EqualityMixin<Point>, Counted<Point> {
    int x = 0;
    int y = 0;
    bool equal_to(const Point& o) const { return x == o.x && y == o.y; }
};

struct Named : EqualityMixin<Named> {
    std::string name;
    bool equal_to(const Named& o) const { return name == o.name; }
};

void demo_basics() {
    Point a;
    a.x = 1;
    a.y = 2;
    Point b;
    b.x = 1;
    b.y = 2;
    Point c;
    c.x = 0;
    c.y = 0;
    LEARN_CHECK(a == b);
    LEARN_CHECK(a != c);
}

void demo_intermediate() {
    LEARN_CHECK(Counted<Point>::live >= 0);
    {
        Point p;
        LEARN_CHECK(Counted<Point>::live >= 1);
    }
    // After scope, live decreases.
    LEARN_CHECK(Counted<Point>::live >= 0);
}

void demo_expert() {
    Named n1;
    n1.name = "a";
    Named n2;
    n2.name = "a";
    Named n3;
    n3.name = "b";
    LEARN_CHECK(n1 == n2);
    LEARN_CHECK(n1 != n3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_inject_behavior", run>;

}  // namespace
