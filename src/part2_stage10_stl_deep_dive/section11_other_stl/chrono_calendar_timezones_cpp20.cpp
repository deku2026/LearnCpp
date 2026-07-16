// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : chrono_calendar_timezones_cpp20
// Topic id : part2/stage10/section11/chrono_calendar_timezones_cpp20
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <chrono>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section11/chrono_calendar_timezones_cpp20";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    using namespace std::chrono;
    constexpr year_month_day leap_day{year{2024}, February, day{29}};
    static_assert(leap_day.ok());
    const sys_days absolute{leap_day};
    const year_month_day next{absolute + days{1}};
    LEARN_EXPECT_EQ(checks, static_cast<unsigned>(next.month()), 3U);
    LEARN_EXPECT_EQ(checks, static_cast<unsigned>(next.day()), 1U);
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    // locate_zone/current_zone depend on an installed time-zone database, so the all-topic run avoids that input.
    static_assert(requires { get_tzdb_list(); });
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage10/section11/chrono_calendar_timezones_cpp20", run>;

}  // namespace
