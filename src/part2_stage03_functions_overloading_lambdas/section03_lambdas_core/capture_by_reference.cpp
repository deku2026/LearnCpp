// Reference capture aliases an existing object; it does not extend lifetime.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : capture_by_reference
// Topic id : part2/stage03/section03/capture_by_reference
//
// The aliasing examples execute only while their referents are alive.  The
// returned callback uses value capture as the safe cross-scope counterpart.

#include "learn/example_support.hpp"

#include <functional>
#include <type_traits>

namespace {

std::function<int()> make_safe_callback() {
    int local = 42;
    return [local] { return local; };
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/capture_by_reference"};

    int total = 10;
    auto add = [&total](int value) {
        total += value;
        return total;
    };
    static_assert(std::is_invocable_r_v<int, decltype(add)&, int>);

    LEARN_EXPECT_EQ(checks, add(5), 15);
    total = 100;
    LEARN_EXPECT_EQ(checks, add(1), 101);

    int left = 2;
    int right = 3;
    auto increment_both = [&] {
        ++left;
        ++right;
    };
    increment_both();
    LEARN_EXPECT_EQ(checks, left, 3);
    LEARN_EXPECT_EQ(checks, right, 4);

    const auto safe_after_return = make_safe_callback();
    LEARN_EXPECT_EQ(checks, safe_after_return(), 42);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_by_reference", run>;

}  // namespace
