// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : chrono_overview
// Topic id : part3/section04/chrono_overview
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <chrono>
#include <concepts>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section04/chrono_overview";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    using namespace std::chrono_literals;
    constexpr auto interval = 1s + 250ms;
    static_assert(std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() == 1'250);
    const auto rounded = std::chrono::round<std::chrono::seconds>(1'600ms);
    const auto floored = std::chrono::floor<std::chrono::seconds>(1'600ms);
    const auto ceiled = std::chrono::ceil<std::chrono::seconds>(1'001ms);
    LEARN_EXPECT_EQ(checks, rounded.count(), 2LL);
    LEARN_EXPECT_EQ(checks, floored.count(), 1LL);
    LEARN_EXPECT_EQ(checks, ceiled.count(), 2LL);
    const std::chrono::steady_clock::time_point start{5s};
    LEARN_EXPECT_EQ(checks, (start + interval - start), interval);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section04/chrono_overview", run>;

}  // namespace
