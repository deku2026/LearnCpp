// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : assume_attribute_cpp23
// Topic id : part2/stage08/section03/assume_attribute_cpp23
//

#include "learn/example_support.hpp"

namespace {

#if __has_cpp_attribute(assume) >= 202207L
[[nodiscard]] int divide_after_validation(const int numerator, const int denominator) noexcept {
    if (denominator <= 0) {
        return 0;
    }

    // An assumption is an optimizer contract, not a runtime check. The guard above
    // establishes the condition before it is promised to the implementation.
    [[assume(denominator > 0)]];
    return numerator / denominator;
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if __has_cpp_attribute(assume) >= 202207L
    learn::ExampleChecks checks{"part2/stage08/section03/assume_attribute_cpp23"};
    LEARN_EXPECT_EQ(checks, divide_after_validation(84, 7), 12);
    LEARN_EXPECT_EQ(checks, divide_after_validation(84, 0), 0);

#if 0
    const int unchecked = 0;
    [[assume(unchecked != 0)]];  // Violating an evaluated assumption has undefined behavior.
    return 84 / unchecked;
#endif

    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage08/section03/assume_attribute_cpp23", "C++23 [[assume]]");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/assume_attribute_cpp23", run>;

}  // namespace
