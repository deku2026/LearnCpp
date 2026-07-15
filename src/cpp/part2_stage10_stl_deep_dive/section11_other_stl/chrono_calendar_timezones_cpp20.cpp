// Topic    : C++20 chrono 日历与时区
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : chrono_calendar_timezones_cpp20
// Topic id : part2/stage10/section11/chrono_calendar_timezones_cpp20
// Refs     : https://en.cppreference.com/w/cpp/chrono/year_month_day
//            https://en.cppreference.com/w/cpp/chrono/zoned_time
//            https://en.cppreference.com/w/cpp/chrono/time_zone

#include "learn/topic_registry.hpp"

#include <chrono>
#include <format>
#include <iostream>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    using namespace std::chrono;
    std::cout << "=== [chrono_calendar_timezones_cpp20] calendar + zones ===\n";

    // ① 日历字面值与 year_month_day
    {
        [[maybe_unused]] const year_month_day ymd = 2025y / June / 16d;
        assert(ymd.ok());
        assert(static_cast<int>(ymd.year()) == 2025);
        assert(static_cast<unsigned>(ymd.month()) == 6);
        assert(static_cast<unsigned>(ymd.day()) == 16);
        std::cout << "  2025y/June/16d ok\n";
    }

    // ② 月末 / 星期（注意: last 是 chrono::last 标签, 勿与变量同名遮蔽）
    {
        const year_month_day_last ymdl = 2024y / February / std::chrono::last;
        assert(ymdl.ok());
        const year_month_day ymd{ymdl};
        // 2024 闰年
        assert(static_cast<unsigned>(ymd.day()) == 29);
        std::cout << "  2024 Feb last day=" << static_cast<unsigned>(ymd.day()) << '\n';

        const year_month_weekday first_monday = 2025y / January / Monday[1];
        assert(first_monday.ok());
        const year_month_day d{first_monday};
        assert(d.ok());
        std::cout << "  2025 first Monday: " << static_cast<unsigned>(d.day()) << " Jan\n";
    }

    // ③ sys_days 互转
    {
        const year_month_day ymd = 2020y / January / 1d;
        const sys_days sd{ymd};
        [[maybe_unused]] const year_month_day back{sd};
        assert(back == ymd);
        std::cout << "  sys_days round-trip OK\n";
    }

    // ④ 时区: current_zone / zoned_time(依赖 tzdb, 实现可能抛异常)
    try {
        const time_zone* tz = current_zone();
        assert(tz != nullptr);
        const auto name = tz->name();
        std::cout << "  current_zone: " << name << '\n';

        const zoned_time zt{tz, system_clock::now()};
        const auto local = zt.get_local_time();
        const auto sys = zt.get_sys_time();
        (void)local;
        (void)sys;
        // format 若可用则打印本地时间
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
        // %F %T 等 chrono 格式说明符
        try {
            const std::string s = std::format("{:%F %T %Z}", zt);
            assert(!s.empty());
            std::cout << "  zoned now: " << s << '\n';
        } catch (const std::exception& ex) {
            std::cout << "  format zoned_time: " << ex.what() << '\n';
        }
#endif
    } catch (const std::exception& ex) {
        // 无 tzdb 的嵌入式/精简环境
        std::cout << "  timezone DB unavailable: " << ex.what() << '\n';
    }

    std::cout << "[chrono_calendar_timezones_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11/chrono_calendar_timezones_cpp20", run>;

}  // namespace
