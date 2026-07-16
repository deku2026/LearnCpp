// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section03_concepts_deep
// Item     : constraint_subsumption
// Topic id : part6/e/section03/constraint_subsumption

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <concepts>
#include <string_view>
#include <type_traits>

namespace {

template <class T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template <class T>
concept SignedArithmetic = Arithmetic<T> && std::is_signed_v<T>;

template <Arithmetic T>
[[nodiscard]] constexpr std::string_view choose(T) noexcept {
    return "arithmetic";
}

template <SignedArithmetic T>
[[nodiscard]] constexpr std::string_view choose(T) noexcept {
    return "signed arithmetic";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section03/constraint_subsumption"};

    // SignedArithmetic reuses the exact Arithmetic atomic constraint, so its
    // overload subsumes and is more constrained than the general overload.
    static_assert(choose(1) == "signed arithmetic");
    static_assert(choose(1U) == "arithmetic");
    static_assert(choose(1.0) == "signed arithmetic");
    LEARN_EXPECT_EQ(checks, choose(2U), std::string_view{"arithmetic"});
    LEARN_EXPECT_EQ(checks, choose(-2), std::string_view{"signed arithmetic"});

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section03/constraint_subsumption", run>;

}  // namespace
