// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : aggregate_vs_non_aggregate
// Topic id : part2/stage04/section01/aggregate_vs_non_aggregate
//
// Covers: aggregate init, what breaks aggregate status, designated init light

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

struct Agg {
    int x;
    int y = 2;
};

class NotAgg {
    int x_ = 0;

public:
    explicit NotAgg(int x) : x_(x) {}
    int x() const { return x_; }
};

struct HasUserCtor {
    int x;
    HasUserCtor(int v) : x(v) {}
};

void demo_basics() {
    Agg a{1};
    LEARN_CHECK(a.x == 1 && a.y == 2);

    Agg b{3, 4};
    LEARN_CHECK(b.x == 3 && b.y == 4);

#if defined(__cpp_designated_initializers)
    Agg c{.x = 9, .y = 8};
    LEARN_CHECK(c.x == 9 && c.y == 8);
#endif
}

void demo_intermediate() {
    static_assert(std::is_aggregate_v<Agg>);
    static_assert(!std::is_aggregate_v<NotAgg>);
    static_assert(!std::is_aggregate_v<HasUserCtor>);

    NotAgg n{5};
    LEARN_CHECK(n.x() == 5);
}

void demo_expert() {
    // User-declared constructors, private/protected non-static data, virtual
    // functions, and virtual bases make a class non-aggregate.
    Agg nested[2]{{1, 2}, {3, 4}};
    LEARN_CHECK(nested[0].x == 1);
    LEARN_CHECK(nested[1].y == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/aggregate_vs_non_aggregate", run>;

}  // namespace
