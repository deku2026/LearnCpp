// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section09_aggregates_and_struct_basics
// Item     : struct_basics
// Topic id : part2/stage02/section09/struct_basics
//
// Covers: struct members, default member init, access, simple methods

#include "learn/topic_registry.hpp"

#include <string>

namespace {

struct Point {
    int x = 0;
    int y = 0;
};

struct Person {
    std::string name;
    int age = 0;

    bool adult() const { return age >= 18; }
};

void demo_basics() {
    Point p;
    LEARN_CHECK(p.x == 0 && p.y == 0);

    p.x = 3;
    p.y = 4;
    LEARN_CHECK(p.x == 3 && p.y == 4);

    Point q{10, 20};
    LEARN_CHECK(q.x == 10 && q.y == 20);
}

void demo_intermediate() {
    Person a{"Ann", 20};
    Person b{"Bob", 15};
    LEARN_CHECK(a.adult());
    LEARN_CHECK(!b.adult());
    LEARN_CHECK(a.name == "Ann");

    // Memberwise copy
    Person c = a;
    LEARN_CHECK(c.name == "Ann" && c.age == 20);
    c.age = 21;
    LEARN_CHECK(a.age == 20);
}

void demo_expert() {
    struct Rect {
        Point origin{};
        int w = 0;
        int h = 0;

        int area() const { return w * h; }
    };

    Rect r{Point{1, 2}, 30, 40};
    LEARN_CHECK(r.origin.x == 1 && r.origin.y == 2);
    LEARN_CHECK(r.w == 30 && r.h == 40);
    LEARN_CHECK(r.area() == 1200);  // 30 * 40

    // Pointer to member access
    Point pt{5, 6};
    Point* pp = &pt;
    LEARN_CHECK(pp->x == 5);
    LEARN_CHECK((*pp).y == 6);

    // Aggregate-ish nested defaulting
    Rect empty{};
    LEARN_CHECK(empty.origin.x == 0 && empty.w == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section09/struct_basics", run>;

}  // namespace
