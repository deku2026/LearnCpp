// C++20 pack init-capture expands one closure member per pack element.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : capture_pack_cpp20
// Topic id : part2/stage03/section03/capture_pack_cpp20
//
// Moving each function argument into the capture makes the resulting closure
// own its entire variadic input.

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace {

template <class... Values>
constexpr auto make_sum(Values... values) {
    return [... parts = std::move(values)] { return (... + parts); };
}

template <class... Values>
auto make_concatenation(Values... values) {
    return [... parts = std::move(values)] { return (std::string{} + ... + parts); };
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/capture_pack_cpp20"};

    constexpr auto sum = make_sum(1, 2, 3, 4);
    static_assert(sum() == 10);
    static_assert(std::is_copy_constructible_v<decltype(sum)>);
    LEARN_EXPECT_EQ(checks, sum(), 10);

    auto text = make_concatenation(std::string{"pack"}, std::string{"-"}, std::string{"capture"});
    LEARN_EXPECT_EQ(checks, text(), std::string{"pack-capture"});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_pack_cpp20", run>;

}  // namespace
