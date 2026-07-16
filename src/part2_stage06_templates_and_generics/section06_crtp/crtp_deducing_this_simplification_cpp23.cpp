// C++23 explicit object parameters remove CRTP's Derived template argument and cast.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_deducing_this_simplification_cpp23
// Topic id : part2/stage06/section06/crtp_deducing_this_simplification_cpp23
//
// P0847R7 is guarded by its SD-6 macro before the syntax is parsed.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L

struct EvaluatorBase {
    constexpr int evaluate(this const auto& self, int value) { return self.evaluate_impl(value); }
};

struct Tripler : EvaluatorBase {
    constexpr int evaluate_impl(int value) const { return value * 3; }
};

struct Offset : EvaluatorBase {
    int amount{};
    constexpr int evaluate_impl(int value) const { return value + amount; }
};

static_assert(Tripler{}.evaluate(4) == 12);
static_assert(std::is_base_of_v<EvaluatorBase, Tripler>);

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    learn::ExampleChecks checks{"part2/stage06/section06/crtp_deducing_this_simplification_cpp23"};
    LEARN_EXPECT_EQ(checks, Tripler{}.evaluate(5), 15);
    LEARN_EXPECT_EQ(checks, (Offset{EvaluatorBase{}, 7}.evaluate(5)), 12);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage06/section06/crtp_deducing_this_simplification_cpp23",
                                             "explicit object parameters (__cpp_explicit_this_parameter >= 202110L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_deducing_this_simplification_cpp23", run>;

}  // namespace
