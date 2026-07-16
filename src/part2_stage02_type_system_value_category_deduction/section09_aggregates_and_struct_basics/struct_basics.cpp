// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section09_aggregates_and_struct_basics
// Item     : struct_basics
// Topic id : part2/stage02/section09/struct_basics
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct Point {
    int x{};
    int y{};

    void translate(int dx, int dy) {
        x += dx;
        y += dy;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_aggregate_v<Point>);
    Point point{2, 3};
    point.translate(4, -1);

    learn::ExampleChecks checks{"part2/stage02/section09/struct_basics"};
    LEARN_EXPECT_EQ(checks, point.x, 6);
    LEARN_EXPECT_EQ(checks, point.y, 2);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section09/struct_basics", run>;

}  // namespace
