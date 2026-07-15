// Topic    : headers map · 时间 time
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : time_headers
// Topic id : part3/section08/time_headers
// Headers  : <chrono>

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
    const auto d = 1500ms;
    assert(std::chrono::duration_cast<std::chrono::seconds>(d).count() == 1);
    std::cout << "1500ms -> " << std::chrono::duration_cast<std::chrono::seconds>(d).count() << "s (trunc)\n";

    const auto t0 = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(1ms);
    const auto t1 = std::chrono::steady_clock::now();
    assert(t1 >= t0);
    std::cout << "steady_clock delta_ns=" << std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()
              << '\n';

    const auto sys_now = std::chrono::system_clock::now();
    const auto tt = std::chrono::system_clock::to_time_t(sys_now);
    std::cout << "system_clock time_t=" << tt << '\n';

    // C++20 日历类型探测
    using std::chrono::day;
    using std::chrono::month;
    using std::chrono::year;
    const std::chrono::year_month_day ymd{year{2026} / month{7} / day{15}};
    assert(ymd.ok());
    std::cout << "ymd ok: " << static_cast<int>(ymd.year()) << '-' << static_cast<unsigned>(ymd.month()) << '-'
              << static_cast<unsigned>(ymd.day()) << '\n';

    std::cout << "[time_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/time_headers", run>;

}  // namespace
