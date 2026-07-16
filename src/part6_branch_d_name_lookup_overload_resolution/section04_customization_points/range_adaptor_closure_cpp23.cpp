// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : range_adaptor_closure_cpp23
// Topic id : part6/d/section04/range_adaptor_closure_cpp23
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <ranges>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/d/section04/range_adaptor_closure_cpp23";

#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202202L

struct Times : std::ranges::range_adaptor_closure<Times> {
    constexpr explicit Times(int value) : factor(value) {}

    int factor{};

    template <std::ranges::viewable_range Range>
    [[nodiscard]] constexpr auto operator()(Range&& range) const {
        return std::forward<Range>(range) |
               std::views::transform([factor = factor](auto value) { return value * factor; });
    }
};

struct Plus : std::ranges::range_adaptor_closure<Plus> {
    constexpr explicit Plus(int value) : increment(value) {}

    int increment{};

    template <std::ranges::viewable_range Range>
    [[nodiscard]] constexpr auto operator()(Range&& range) const {
        return std::forward<Range>(range) |
               std::views::transform([increment = increment](auto value) { return value + increment; });
    }
};

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202202L
    ::learn::ExampleChecks checks{kTopic};
    const std::vector values{1, 2, 3};

    const auto transformed = values | Times{2} | Plus{1};
    int expected = 3;
    int sum{};
    for (const int value : transformed) {
        LEARN_EXPECT_EQ(checks, value, expected);
        expected += 2;
        sum += value;
    }
    LEARN_EXPECT_EQ(checks, sum, 15);

    // Closure objects also compose before being applied to a range.
    const auto composed = Times{3} | Plus{-1};
    const auto second = values | composed;
    LEARN_EXPECT_EQ(checks, *second.begin(), 2);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "C++23 range_adaptor_closure");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section04/range_adaptor_closure_cpp23", run>;

}  // namespace
