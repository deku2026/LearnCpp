// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : extended_floating_types_cpp23
// Topic id : part3/section04/extended_floating_types_cpp23
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <concepts>
#include <string_view>

#if __has_include(<stdfloat>)
#include <stdfloat>
#endif
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part3/section04/extended_floating_types_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if __has_include(<stdfloat>) && defined(__STDCPP_FLOAT32_T__)
    ::learn::ExampleChecks checks{kTopic};
    static_assert(std::is_floating_point_v<std::float32_t>);
    const std::float32_t value = 1.5F32;
    LEARN_EXPECT(checks, value > std::float32_t{1.0F32});
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "C++23 extended floating-point typedefs");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section04/extended_floating_types_cpp23", run>;

}  // namespace
