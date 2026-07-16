// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : numerics_headers
// Topic id : part3/section08/numerics_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <complex>
#include <limits>
#include <numbers>
#include <numeric>
#include <random>
#include <ratio>
#include <string_view>
#include <valarray>

namespace {

constexpr std::string_view kTopic = "part3/section08/numerics_headers";

constexpr std::array<std::string_view, 12> headers{
    "bit",     "cfenv",  "cmath", "complex",  "limits",   "numbers",
    "numeric", "random", "ratio", "valarray", "stdfloat", "version",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, std::gcd(12, 18), 6);
    LEARN_EXPECT_EQ(checks, std::popcount(0b1011U), 3);
    const std::complex<double> value{3.0, 4.0};
    LEARN_EXPECT_EQ(checks, std::abs(value), 5.0);
    LEARN_EXPECT(checks, std::numbers::pi > 3.14);
    LEARN_EXPECT_EQ(checks, headers.size(), 12U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/numerics_headers", run>;

}  // namespace
