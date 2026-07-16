// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : numbers_constants
// Topic id : part3/section04/numbers_constants
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <cmath>
#include <concepts>
#include <numbers>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section04/numbers_constants";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    static_assert(std::numbers::pi_v<double> > 3.14 && std::numbers::pi_v<double> < 3.15);
    LEARN_EXPECT(checks, std::abs(std::numbers::sqrt2 * std::numbers::sqrt2 - 2.0) < 1e-12);
    LEARN_EXPECT(checks, std::abs(std::numbers::inv_pi * std::numbers::pi - 1.0) < 1e-12);
    LEARN_EXPECT(checks, std::numbers::e > 2.71 && std::numbers::e < 2.72);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section04/numbers_constants", run>;

}  // namespace
