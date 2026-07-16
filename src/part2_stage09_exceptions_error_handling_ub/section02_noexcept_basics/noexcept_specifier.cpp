// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section02_noexcept_basics
// Item     : noexcept_specifier
// Topic id : part2/stage09/section02/noexcept_specifier
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section02/noexcept_specifier";

struct StableSwap {
    int value{};
    friend void swap(StableSwap& left, StableSwap& right) noexcept { std::swap(left.value, right.value); }
};

struct PotentiallyThrowingSwap {
    int value{};
    friend void swap(PotentiallyThrowingSwap& left, PotentiallyThrowingSwap& right) {
        std::swap(left.value, right.value);
    }
};

template <class T>
void exchange_values(T& left, T& right) noexcept(noexcept(swap(left, right))) {
    swap(left, right);
}

static_assert(noexcept(exchange_values(std::declval<StableSwap&>(), std::declval<StableSwap&>())));
static_assert(!noexcept(exchange_values(std::declval<PotentiallyThrowingSwap&>(),
                                        std::declval<PotentiallyThrowingSwap&>())));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    StableSwap left{1};
    StableSwap right{2};
    exchange_values(left, right);
    LEARN_EXPECT_EQ(checks, left.value, 2);
    LEARN_EXPECT_EQ(checks, right.value, 1);
    PotentiallyThrowingSwap first{3};
    PotentiallyThrowingSwap second{4};
    exchange_values(first, second);
    LEARN_EXPECT_EQ(checks, first.value, 4);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section02/noexcept_specifier", run>;

}  // namespace
