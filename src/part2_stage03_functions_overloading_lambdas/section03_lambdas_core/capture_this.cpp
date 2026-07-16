// [this] stores the current object's pointer, not an object snapshot.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : capture_this
// Topic id : part2/stage03/section03/capture_this
//
// Copies of the closure therefore continue to operate on the same object.
// Every invocation below occurs before that object is destroyed.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct Accumulator {
    int total{};

    auto make_adder() {
        return [this](int value) {
            total += value;
            return total;
        };
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/capture_this"};

    Accumulator accumulator{10};
    auto add = accumulator.make_adder();
    auto copied_closure = add;
    static_assert(std::is_copy_constructible_v<decltype(add)>);

    LEARN_EXPECT_EQ(checks, add(2), 12);
    LEARN_EXPECT_EQ(checks, copied_closure(3), 15);
    LEARN_EXPECT_EQ(checks, accumulator.total, 15);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_this", run>;

}  // namespace
