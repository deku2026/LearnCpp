// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : std_forward_deduction_basis
// Topic id : part2/stage02/section06/std_forward_deduction_basis
//

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view category(int&) {
    return "lvalue";
}

constexpr std::string_view category(int&&) {
    return "rvalue";
}

template <class T>
constexpr std::string_view forwarded_category(T&& value) {
    static_assert(std::is_same_v<decltype(std::forward<T>(value)), T&&>);
    return category(std::forward<T>(value));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int value = 3;
    learn::ExampleChecks checks{"part2/stage02/section06/std_forward_deduction_basis"};
    LEARN_EXPECT_EQ(checks, forwarded_category(value), std::string_view{"lvalue"});
    LEARN_EXPECT_EQ(checks, forwarded_category(4), std::string_view{"rvalue"});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/std_forward_deduction_basis", run>;

}  // namespace
