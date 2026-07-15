// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : pass_this_by_value
// Topic id : part2/stage04/section04/pass_this_by_value
//
// Covers: pass explicit object parameter by value for small types

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

#if defined(__cpp_explicit_this_parameter)

struct Point {
    int x = 0;
    int y = 0;

    int sum(this Point self) { return self.x + self.y; }

    Point shifted(this Point self, int dx, int dy) {
        self.x += dx;
        self.y += dy;
        return self;
    }
};

void demo_basics() {
    Point p{3, 4};
    assert(p.sum() == 7);
}

void demo_intermediate() {
    Point p{1, 2};
    Point q = p.shifted(3, 5);
    assert(q.x == 4 && q.y == 7);
    // by-value self: original unchanged
    assert(p.x == 1 && p.y == 2);
}

void demo_expert() {
    assert((Point{10, -3}.sum() == 7));
}

#else

struct Point {
    int x = 0;
    int y = 0;
    int sum() const { return x + y; }
};

void demo_basics() {
    assert(Point{3, 4}.sum() == 7);
}

void demo_intermediate() {
    assert(true);
}

void demo_expert() {
    assert(true);
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/pass_this_by_value", run>;

}  // namespace
