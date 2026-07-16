// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : time_headers
// Topic id : part3/section08/time_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <chrono>
#include <ctime>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section08/time_headers";

constexpr std::array<std::string_view, 2> headers{"chrono", "ctime"};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    using namespace std::chrono_literals;
    constexpr auto duration = 2min + 30s;
    LEARN_EXPECT_EQ(checks, std::chrono::duration_cast<std::chrono::seconds>(duration).count(), 150LL);
    const std::chrono::sys_days day = std::chrono::year{2026} / std::chrono::July / 16;
    const std::chrono::year_month_day calendar{day};
    LEARN_EXPECT_EQ(checks, static_cast<unsigned>(calendar.day()), 16U);
    LEARN_EXPECT_EQ(checks, headers.size(), 2U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/time_headers", run>;

}  // namespace
