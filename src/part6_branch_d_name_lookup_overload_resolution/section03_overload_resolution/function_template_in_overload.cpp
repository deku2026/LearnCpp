// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : function_template_in_overload
// Topic id : part6/d/section03/function_template_in_overload
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <concepts>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section03/function_template_in_overload";

enum class Choice { non_template, generic_template, integral_template };

[[nodiscard]] constexpr Choice choose(int) {
    return Choice::non_template;
}

template <class T>
[[nodiscard]] constexpr Choice choose(T) {
    return Choice::generic_template;
}

template <std::integral T>
    requires(!std::same_as<T, int>)
[[nodiscard]] constexpr Choice choose(T) {
    return Choice::integral_template;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    // If conversion sequences tie, a non-template beats a template.
    LEARN_EXPECT(checks, choose(1) == Choice::non_template);
    // A more constrained viable template wins over the unconstrained one.
    LEARN_EXPECT(checks, choose(1L) == Choice::integral_template);
    // Exact template match beats converting double to the int overload.
    LEARN_EXPECT(checks, choose(1.5) == Choice::generic_template);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section03/function_template_in_overload", run>;

}  // namespace
