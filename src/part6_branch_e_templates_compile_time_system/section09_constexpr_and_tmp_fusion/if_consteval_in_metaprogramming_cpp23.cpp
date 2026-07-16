// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section09_constexpr_and_tmp_fusion
// Item     : if_consteval_in_metaprogramming_cpp23
// Topic id : part6/e/section09/if_consteval_in_metaprogramming_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

[[nodiscard]] constexpr int phase_sensitive(int value) noexcept {
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    if consteval {
        return value * 2;
    } else {
        return value * 3;
    }
#else
    return std::is_constant_evaluated() ? value * 2 : value * 3;
#endif
}

template <int Value>
inline constexpr int compile_time_route = phase_sensitive(Value);

int run(int argc, char** argv) {
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section09/if_consteval_in_metaprogramming_cpp23"};

    static_assert(phase_sensitive(21) == 42);
    static_assert(compile_time_route<10> == 20);
    const int runtime_input = argc + 4;
    LEARN_EXPECT_EQ(checks, phase_sensitive(runtime_input), runtime_input * 3);

#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    constexpr bool has_if_consteval = true;
#else
    constexpr bool has_if_consteval = false;
#endif
    constexpr auto status = has_if_consteval ? "native if consteval" : "is_constant_evaluated fallback";
    LEARN_EXPECT(checks, status[0] != '\0');

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section09/if_consteval_in_metaprogramming_cpp23", run>;

}  // namespace
