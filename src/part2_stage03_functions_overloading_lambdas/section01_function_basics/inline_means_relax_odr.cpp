// inline is primarily an ODR facility, not an inlining command.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : inline_means_relax_odr
// Topic id : part2/stage03/section01/inline_means_relax_odr
//
// These definitions have the form used in a header: every translation unit
// may contain the same definition and still denote one entity program-wide.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

namespace header_style {

inline constexpr int offset = 3;
inline int call_count = 0;

inline int adjusted_square(int value) {
    ++call_count;
    return value * value + offset;
}

}  // namespace header_style

static_assert(header_style::offset == 3);
static_assert(std::is_same_v<decltype(header_style::offset), const int>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage03/section01/inline_means_relax_odr"};
    header_style::call_count = 0;

    auto* first_address = &header_style::call_count;
    LEARN_EXPECT_EQ(checks, header_style::adjusted_square(4), 19);
    LEARN_EXPECT_EQ(checks, header_style::adjusted_square(5), 28);
    auto* second_address = &header_style::call_count;

    LEARN_EXPECT(checks, first_address == second_address);
    LEARN_EXPECT_EQ(checks, header_style::call_count, 2);
    // Whether either call is physically expanded is still the optimizer's choice.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/inline_means_relax_odr", run>;

}  // namespace
