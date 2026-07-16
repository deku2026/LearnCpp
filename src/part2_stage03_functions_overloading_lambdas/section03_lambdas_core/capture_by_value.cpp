// Value capture stores a snapshot inside the closure object.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : capture_by_value
// Topic id : part2/stage03/section03/capture_by_value
//
// Later changes to the source variables are not observed, and the closure's
// default call operator can read its captured members through a const object.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/capture_by_value"};

    int width = 3;
    int height = 4;
    const auto captured_area = [width, height] { return width * height; };
    static_assert(std::is_invocable_r_v<int, const decltype(captured_area)&>);
    static_assert(std::is_copy_constructible_v<decltype(captured_area)>);

    width = 30;
    height = 40;
    LEARN_EXPECT_EQ(checks, captured_area(), 12);
    LEARN_EXPECT_EQ(checks, width * height, 1200);

    const int bias = 5;
    const auto default_capture = [=](int value) { return value + bias; };
    LEARN_EXPECT_EQ(checks, default_capture(7), 12);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_by_value", run>;

}  // namespace
