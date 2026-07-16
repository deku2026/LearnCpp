// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section05_value_categories
// Item     : simplified_implicit_move_cpp23
// Topic id : part2/stage02/section05/simplified_implicit_move_cpp23
//

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

struct MoveOnly {
    explicit MoveOnly(int input) : value(input) {}
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&& other) noexcept : value(std::exchange(other.value, -1)) {}
    MoveOnly& operator=(MoveOnly&&) = delete;

    int value;
};

#if defined(__cpp_implicit_move) && __cpp_implicit_move >= 202207L
MoveOnly relay(MoveOnly value) {
    return value;  // The move-eligible id-expression is treated as an xvalue.
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(!std::is_copy_constructible_v<MoveOnly>);
    static_assert(std::is_move_constructible_v<MoveOnly>);

#if defined(__cpp_implicit_move) && __cpp_implicit_move >= 202207L
    MoveOnly source{42};
    MoveOnly result = relay(std::move(source));

    learn::ExampleChecks checks{"part2/stage02/section05/simplified_implicit_move_cpp23"};
    LEARN_EXPECT_EQ(checks, result.value, 42);
    LEARN_EXPECT_EQ(checks, source.value, -1);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section05/simplified_implicit_move_cpp23",
                                             "__cpp_implicit_move");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/simplified_implicit_move_cpp23", run>;

}  // namespace
