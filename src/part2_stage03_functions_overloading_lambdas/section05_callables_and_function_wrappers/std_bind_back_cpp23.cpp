// C++23 bind_back fixes the final arguments of a callable.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_bind_back_cpp23
// Topic id : part2/stage03/section05/std_bind_back_cpp23
//
// Invocation is equivalent to invoke(f, call_arguments..., bound_arguments...).

#include "learn/example_support.hpp"

#include <functional>
#include <string>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage03/section05/std_bind_back_cpp23";

int affine(int value, int scale, int offset) {
    return value * scale + offset;
}

std::string surround(std::string_view text, std::string_view left, std::string_view right) {
    return std::string{left} + std::string{text} + std::string{right};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{kTopic};
#if defined(__cpp_lib_bind_back) && __cpp_lib_bind_back >= 202202L
    auto twice_plus_three = std::bind_back(affine, 2, 3);
    static_assert(std::is_invocable_r_v<int, decltype(twice_plus_three)&, int>);
    LEARN_EXPECT_EQ(checks, twice_plus_three(5), 13);

    auto close_bracket = std::bind_back(surround, std::string_view{"]"});
    LEARN_EXPECT_EQ(checks, close_bracket(std::string_view{"value"}, std::string_view{"["}), std::string{"[value]"});
    return checks.result();
#else
    // Model the same final-argument binding with lambdas.  Keeping a checked
    // fallback both teaches the transformation and exercises the helpers on
    // standard libraries that do not yet ship bind_back.
    auto twice_plus_three = [](int value) { return affine(value, 2, 3); };
    static_assert(std::is_invocable_r_v<int, decltype(twice_plus_three)&, int>);
    LEARN_EXPECT_EQ(checks, twice_plus_three(5), 13);

    auto close_bracket = [](std::string_view text, std::string_view left) {
        return surround(text, left, std::string_view{"]"});
    };
    LEARN_EXPECT_EQ(checks, close_bracket(std::string_view{"value"}, std::string_view{"["}), std::string{"[value]"});
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(kTopic, "std::bind_back (__cpp_lib_bind_back >= 202202L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_bind_back_cpp23", run>;

}  // namespace
