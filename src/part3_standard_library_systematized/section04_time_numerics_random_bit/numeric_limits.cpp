// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : numeric_limits
// Topic id : part3/section04/numeric_limits
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <cmath>
#include <concepts>
#include <limits>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part3/section04/numeric_limits";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    static_assert(std::numeric_limits<unsigned>::is_modulo);
    static_assert(!std::numeric_limits<int>::is_modulo);
    static_assert(std::numeric_limits<double>::has_infinity);
    LEARN_EXPECT(checks, std::numeric_limits<int>::lowest() < 0);
    LEARN_EXPECT(checks, std::numeric_limits<int>::max() > 0);
    LEARN_EXPECT(checks, std::isinf(std::numeric_limits<double>::infinity()));
    LEARN_EXPECT(checks, std::isnan(std::numeric_limits<double>::quiet_NaN()));
    LEARN_EXPECT(checks, std::numeric_limits<double>::epsilon() > 0.0);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section04/numeric_limits", run>;

}  // namespace
