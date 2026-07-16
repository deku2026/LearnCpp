// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : std_move_static_cast_to_rvalue
// Topic id : part2/stage02/section06/std_move_static_cast_to_rvalue
//

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

struct MoveProbe {
    explicit MoveProbe(int input) : value(input) {}
    MoveProbe(const MoveProbe&) = delete;
    MoveProbe(MoveProbe&& other) noexcept : value(std::exchange(other.value, -1)) {}

    int value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    MoveProbe source{12};
    auto&& cast_only = std::move(source);
    static_assert(std::is_same_v<decltype(std::move(source)), MoveProbe&&>);
    static_assert(std::is_same_v<decltype(static_cast<MoveProbe&&>(source)), MoveProbe&&>);

    learn::ExampleChecks checks{"part2/stage02/section06/std_move_static_cast_to_rvalue"};
    LEARN_EXPECT(checks, &cast_only == &source);
    LEARN_EXPECT_EQ(checks, source.value, 12);  // The cast alone moves nothing.

    MoveProbe destination{std::move(source)};
    LEARN_EXPECT_EQ(checks, destination.value, 12);
    LEARN_EXPECT_EQ(checks, source.value, -1);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/std_move_static_cast_to_rvalue", run>;

}  // namespace
