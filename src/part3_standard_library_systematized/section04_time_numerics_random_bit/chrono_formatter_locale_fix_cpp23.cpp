// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : chrono_formatter_locale_fix_cpp23
// Topic id : part3/section04/chrono_formatter_locale_fix_cpp23
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <chrono>
#include <concepts>
#include <string_view>
#if __has_include(<format>)
#include <format>
#endif
#include <locale>
#include <string>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section04/chrono_formatter_locale_fix_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    ::learn::ExampleChecks checks{kTopic};
    using namespace std::chrono;
    const sys_days date = year{2026} / July / 16;
    LEARN_EXPECT_EQ(checks, std::format("{:%F}", date), std::string{"2026-07-16"});
    LEARN_EXPECT_EQ(checks, std::format(std::locale::classic(), "{:%Y}", date), std::string{"2026"});
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "chrono formatter");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section04/chrono_formatter_locale_fix_cpp23", run>;

}  // namespace
