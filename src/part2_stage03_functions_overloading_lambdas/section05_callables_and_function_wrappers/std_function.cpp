// std::function provides copyable type erasure for one call signature.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_function
// Topic id : part2/stage03/section05/std_function
//
// Different callable types fit one container.  Copies own independent copies
// of stored state, and invoking an empty wrapper throws bad_function_call.

#include "learn/example_support.hpp"

#include <functional>
#include <type_traits>
#include <vector>

namespace {

int twice(int value) {
    return value * 2;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section05/std_function"};

    std::vector<std::function<int(int)>> operations;
    operations.emplace_back(twice);
    operations.emplace_back([](int value) { return value + 3; });
    LEARN_EXPECT_EQ(checks, operations[0](5), 10);
    LEARN_EXPECT_EQ(checks, operations[1](5), 8);

    std::function<int()> counter = [value = 0]() mutable { return ++value; };
    auto copied_counter = counter;
    static_assert(std::is_copy_constructible_v<decltype(counter)>);
    LEARN_EXPECT_EQ(checks, counter(), 1);
    LEARN_EXPECT_EQ(checks, counter(), 2);
    LEARN_EXPECT_EQ(checks, copied_counter(), 1);

    std::function<void()> empty;
    LEARN_EXPECT(checks, !empty);
    LEARN_EXPECT_THROWS(checks, std::bad_function_call, empty());
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_function", run>;

}  // namespace
