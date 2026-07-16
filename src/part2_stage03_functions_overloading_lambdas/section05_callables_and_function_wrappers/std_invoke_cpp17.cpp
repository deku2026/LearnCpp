// C++17 std::invoke gives one syntax to every standard callable category.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_invoke_cpp17
// Topic id : part2/stage03/section05/std_invoke_cpp17
//
// It handles free functions, lambdas, member functions, and member data through
// objects, pointers, and reference_wrapper.

#include "learn/example_support.hpp"

#include <functional>
#include <type_traits>

namespace {

int doubled(int value) {
    return value * 2;
}

struct Record {
    int value{};

    int plus(int amount) const { return value + amount; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section05/std_invoke_cpp17"};
    Record record{40};

    static_assert(std::is_invocable_r_v<int, decltype(&doubled), int>);
    static_assert(std::is_invocable_r_v<int, decltype(&Record::plus), const Record&, int>);
    LEARN_EXPECT_EQ(checks, std::invoke(doubled, 5), 10);
    LEARN_EXPECT_EQ(checks, std::invoke([](int left, int right) { return left + right; }, 2, 3), 5);
    LEARN_EXPECT_EQ(checks, std::invoke(&Record::plus, record, 2), 42);
    LEARN_EXPECT_EQ(checks, std::invoke(&Record::plus, std::cref(record), 3), 43);
    LEARN_EXPECT_EQ(checks, std::invoke(&Record::plus, &record, 4), 44);

    std::invoke(&Record::value, record) = 50;
    LEARN_EXPECT_EQ(checks, record.value, 50);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_invoke_cpp17", run>;

}  // namespace
