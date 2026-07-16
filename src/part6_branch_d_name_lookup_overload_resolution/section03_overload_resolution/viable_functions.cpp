// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : viable_functions
// Topic id : part6/d/section03/viable_functions
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <concepts>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/d/section03/viable_functions";

struct Formatter {
    [[nodiscard]] constexpr int operator()(int value) const { return value; }

    template <std::integral T>
    [[nodiscard]] constexpr int operator()(T left, T right) const {
        return static_cast<int>(left + right);
    }
};

static_assert(std::invocable<Formatter, int>);
static_assert(std::invocable<Formatter, short, short>);
static_assert(!std::invocable<Formatter>);
static_assert(!std::invocable<Formatter, std::string_view, std::string_view>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const Formatter format;
    LEARN_EXPECT_EQ(checks, format(5), 5);
    LEARN_EXPECT_EQ(checks, format(2, 3), 5);
    // Viability filters by argument count, conversions, constraints, and
    // accessibility before a best candidate can be chosen.
    LEARN_EXPECT(checks, (std::is_invocable_v<Formatter, long, long>));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section03/viable_functions", run>;

}  // namespace
