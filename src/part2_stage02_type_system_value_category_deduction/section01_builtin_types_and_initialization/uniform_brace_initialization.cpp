// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : uniform_brace_initialization
// Topic id : part2/stage02/section01/uniform_brace_initialization
//

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace {

struct Coordinate {
    int x;
    int y;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int scalar{4};
    Coordinate point{2, 3};
    std::array<int, 3> array{1, 2, 3};
    std::vector<int> dynamic{4, 5, 6};
    static_assert(std::is_aggregate_v<Coordinate>);

    learn::ExampleChecks checks{"part2/stage02/section01/uniform_brace_initialization"};
    LEARN_EXPECT_EQ(checks, scalar, 4);
    LEARN_EXPECT_EQ(checks, point.x + point.y, 5);
    LEARN_EXPECT_EQ(checks, array[2], 3);
    LEARN_EXPECT_EQ(checks, dynamic.size(), std::size_t{3});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/uniform_brace_initialization", run>;

}  // namespace
