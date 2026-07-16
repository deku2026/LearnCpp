// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : while_and_do_while
// Topic id : part2/stage08/section01/while_and_do_while
//

#include "learn/example_support.hpp"

namespace {

[[nodiscard]] constexpr unsigned factorial(unsigned value) noexcept {
    unsigned result = 1;
    while (value > 1) {
        result *= value;
        --value;
    }
    return result;
}

[[nodiscard]] constexpr unsigned decimal_digits(unsigned value) noexcept {
    unsigned digits = 0;
    do {
        ++digits;
        value /= 10;
    } while (value != 0);
    return digits;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section01/while_and_do_while"};
    static_assert(factorial(5) == 120);
    static_assert(decimal_digits(0) == 1);
    LEARN_EXPECT_EQ(checks, factorial(0), 1U);
    LEARN_EXPECT_EQ(checks, factorial(6), 720U);
    LEARN_EXPECT_EQ(checks, decimal_digits(0), 1U);
    LEARN_EXPECT_EQ(checks, decimal_digits(12'345), 5U);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/while_and_do_while", run>;

}  // namespace
