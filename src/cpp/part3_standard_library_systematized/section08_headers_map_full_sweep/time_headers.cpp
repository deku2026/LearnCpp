// Topic    : headers map · 时间 time
// Doc      : 第3部分-标准库系统化.md · headers map + 库4.1
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : time_headers
// Topic id : part3/section08/time_headers
// Headers  : <chrono>
//
// 要点: duration/clock/time_point + C++20 日历; system≠steady 用途。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [time_headers] map probe ===\n";
    std::cout << "<chrono> durations / clocks / time_point\n";
    std::cout << "C++20: calendar (year_month_day), time zones, formatters\n";

    using namespace std::chrono_literals;

    // duration 换算(截断向零)
    const auto d = 1500ms;
    assert(std::chrono::duration_cast<std::chrono::seconds>(d).count() == 1);
    assert((1500ms + 500ms).count() == 2000);
    assert(std::chrono::duration_cast<std::chrono::milliseconds>(2s).count() == 2000);
    std::cout << "1500ms -> " << std::chrono::duration_cast<std::chrono::seconds>(d).count() << "s (trunc)\n";

    // steady_clock: 测间隔(单调); 勿用 system_clock 测耗时
    const auto t0 = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(1ms);
    const auto t1 = std::chrono::steady_clock::now();
    assert(t1 >= t0);
    const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
    assert(ns >= 0);
    std::cout << "steady_clock delta_ns=" << ns << '\n';

    // system_clock: 墙钟, 可转 time_t; 可能 NTP 回拨
    const auto sys_now = std::chrono::system_clock::now();
    const auto tt = std::chrono::system_clock::to_time_t(sys_now);
    assert(tt > 0);
    std::cout << "system_clock time_t=" << tt << '\n';

    // C++20 日历
    using std::chrono::day;
    using std::chrono::month;
    using std::chrono::year;
    const std::chrono::year_month_day ymd{year{2026} / month{7} / day{15}};
    assert(ymd.ok());
    assert(static_cast<int>(ymd.year()) == 2026);
    assert(static_cast<unsigned>(ymd.month()) == 7);
    assert(static_cast<unsigned>(ymd.day()) == 15);
    std::cout << "ymd ok: " << static_cast<int>(ymd.year()) << '-' << static_cast<unsigned>(ymd.month()) << '-'
              << static_cast<unsigned>(ymd.day()) << '\n';

    // high_resolution 常是 steady 或 system 别名 — 只作探测
    using hr = std::chrono::high_resolution_clock;
    [[maybe_unused]] const auto hr0 = hr::now();
    assert(hr0.time_since_epoch().count() != 0 || true);
    std::cout << "high_resolution is_steady=" << std::boolalpha << hr::is_steady << '\n';
    std::cout << "prefer steady_clock for benchmarks; system_clock for wall time\n";

    std::cout << "[time_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/time_headers", run>;

}  // namespace
