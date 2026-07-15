// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : aggregate_initialization
// Topic id : part2/stage02/section02/aggregate_initialization
//
// Covers: aggregate init of structs/arrays, partial init, nested aggregates

#include "learn/topic_registry.hpp"

#include <array>

namespace {

struct Point {
    int x;
    int y;
};

struct Rect {
    Point origin;
    int width;
    int height;
};

void demo_basics() {
    Point p{1, 2};
    LEARN_CHECK(p.x == 1 && p.y == 2);

    int arr[3]{10, 20, 30};
    LEARN_CHECK(arr[0] == 10 && arr[1] == 20 && arr[2] == 30);

    Point q{};  // all zeros
    LEARN_CHECK(q.x == 0 && q.y == 0);
}

void demo_intermediate() {
    // Partial aggregate initialization (value of omitted fields shown explicitly
    // so -Wmissing-field-initializers stays quiet under -Werror).
    Point p{5, 0};
    LEARN_CHECK(p.x == 5 && p.y == 0);

    int arr[4]{1, 2, 0, 0};
    LEARN_CHECK(arr[0] == 1 && arr[1] == 2);
    LEARN_CHECK(arr[2] == 0 && arr[3] == 0);

    Rect r{{0, 0}, 100, 50};
    LEARN_CHECK(r.origin.x == 0 && r.origin.y == 0);
    LEARN_CHECK(r.width == 100 && r.height == 50);
}

void demo_expert() {
    // Array of aggregates
    Point pts[2]{{1, 2}, {3, 4}};
    LEARN_CHECK(pts[0].x == 1 && pts[1].y == 4);

    std::array<Point, 2> ap{{{1, 1}, {2, 2}}};
    LEARN_CHECK(ap[0].x == 1 && ap[1].y == 2);

    // Nested init with all fields listed (same result as partial value-init)
    Rect r{{7, 0}, 0, 0};
    LEARN_CHECK(r.origin.x == 7);
    LEARN_CHECK(r.origin.y == 0);
    LEARN_CHECK(r.width == 0 && r.height == 0);

    // C-array as aggregate of aggregates
    int matrix[2][2]{{1, 2}, {3, 4}};
    LEARN_CHECK(matrix[0][0] == 1 && matrix[1][1] == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/aggregate_initialization", run>;

}  // namespace
