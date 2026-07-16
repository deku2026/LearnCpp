// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : sfinae_if_constexpr_branch
// Topic id : part2/stage15/section03/sfinae_if_constexpr_branch
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section03/sfinae_if_constexpr_branch";

template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
std::string legacy_classify(T) {
    return "integral";
}

template <class T>
std::string modern_classify(const T&) {
    if constexpr (std::is_integral_v<T>)
        return "integral";
    else if constexpr (requires(const T& value) { value.size(); })
        return "sized";
    else
        return "other";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, legacy_classify(3), std::string{"integral"});
    LEARN_EXPECT_EQ(checks, modern_classify(3), std::string{"integral"});
    LEARN_EXPECT_EQ(checks, modern_classify(std::string{"text"}), std::string{"sized"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section03/sfinae_if_constexpr_branch", run>;

}  // namespace
