// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section03_consteval_constinit
// Item     : consteval_propagation_immediate_escalating_cpp23
// Topic id : part2/stage07/section03/consteval_propagation_immediate_escalating_cpp23
// References: N4950 [expr.const], [dcl.constexpr]; P2564R3.

#include "learn/example_support.hpp"

namespace {

#if defined(__cpp_consteval) && __cpp_consteval >= 202211L

consteval int immediate_twice(int value) noexcept {
    return value * 2;
}

// This constexpr function template specialization is immediate-escalating:
// its non-constant immediate invocation causes the specialization to behave
// as an immediate function rather than making the definition ill-formed.
template <class Value>
constexpr int escalated_template(Value value) noexcept {
    return immediate_twice(static_cast<int>(value));
}

inline constexpr auto escalated_lambda = [](int value) noexcept { return immediate_twice(value) + 1; };

static_assert(escalated_template(21) == 42);
static_assert(escalated_lambda(20) == 41);

#if 0
int runtime_value = 21;
int rejected = escalated_template(runtime_value);  // The specialization has escalated.
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_consteval) && __cpp_consteval >= 202211L
    ::learn::ExampleChecks checks{"part2/stage07/section03/consteval_propagation_immediate_escalating_cpp23"};

    constexpr int template_result = escalated_template(21);
    constexpr int lambda_result = escalated_lambda(20);
    LEARN_EXPECT_EQ(checks, template_result, 42);
    LEARN_EXPECT_EQ(checks, lambda_result, 41);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(
        "part2/stage07/section03/consteval_propagation_immediate_escalating_cpp23",
        "__cpp_consteval >= 202211L (P2564)");
#endif
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section03/consteval_propagation_immediate_escalating_cpp23", run>;

}  // namespace
