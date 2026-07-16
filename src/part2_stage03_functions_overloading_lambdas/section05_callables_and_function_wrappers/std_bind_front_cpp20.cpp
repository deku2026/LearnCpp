// C++20 bind_front fixes the leading arguments of a callable.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_bind_front_cpp20
// Topic id : part2/stage03/section05/std_bind_front_cpp20
//
// Invocation is equivalent to invoke(f, bound_arguments..., call_arguments...).

#include "learn/example_support.hpp"

#include <functional>
#include <type_traits>

namespace {

int subtract(int left, int right) {
    return left - right;
}

struct Ledger {
    int balance{};

    int deposit(int amount) {
        balance += amount;
        return balance;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_bind_front) && __cpp_lib_bind_front >= 201907L
    learn::ExampleChecks checks{"part2/stage03/section05/std_bind_front_cpp20"};

    auto subtract_from_ten = std::bind_front(subtract, 10);
    static_assert(std::is_invocable_r_v<int, decltype(subtract_from_ten)&, int>);
    LEARN_EXPECT_EQ(checks, subtract_from_ten(3), 7);

    Ledger ledger{100};
    // reference_wrapper preserves the object-reference semantics without
    // storing a raw pointer in the bind state.
    auto deposit = std::bind_front(&Ledger::deposit, std::ref(ledger));
    LEARN_EXPECT_EQ(checks, deposit(25), 125);
    LEARN_EXPECT_EQ(checks, ledger.balance, 125);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage03/section05/std_bind_front_cpp20",
                                             "std::bind_front (__cpp_lib_bind_front >= 201907L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_bind_front_cpp20", run>;

}  // namespace
