// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库4 · 4.1 chrono 概览
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : chrono_overview
// Topic id : part3/section04/chrono_overview
//
// Refs:
//   https://en.cppreference.com/w/cpp/chrono
//
// 要点: duration / time_point / clock; 测时用 steady_clock; 墙钟用 system_clock;
//       C++20 日历与时区。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    std::cout << "=== part3/section04/chrono_overview ===\n";

    // 时长字面值与运算(类型安全, 不会混单位)
    const auto total = 2h + 30min + 15s;
    assert(duration_cast<seconds>(total).count() == 2 * 3600 + 30 * 60 + 15);
    std::cout << "[intro] 2h+30min+15s = " << duration_cast<seconds>(total).count() << "s\n";

    // 测时长: steady_clock(单调)
    const auto start = steady_clock::now();
    std::this_thread::sleep_for(5ms);
    const auto elapsed = steady_clock::now() - start;
    assert(elapsed >= 5ms);
    std::cout << "[intro] sleep measured ~" << duration_cast<milliseconds>(elapsed).count() << "ms\n";

    // 墙钟: system_clock — 可调, 不适合 benchmark
    const system_clock::time_point wall = system_clock::now();
    const std::time_t tt = system_clock::to_time_t(wall);
    assert(tt != 0 || tt == 0);  // 可移植地“用到”返回值
    (void)tt;

    // duration_cast 截断向零
    assert(duration_cast<seconds>(1500ms).count() == 1);

    // C++20 日历类型
    [[maybe_unused]] const year_month_day ymd = 2025y / June / 16d;
    assert(ymd.ok());
    assert(static_cast<int>(ymd.year()) == 2025);
    assert(unsigned{ymd.month()} == 6);
    assert(unsigned{ymd.day()} == 16);
    std::cout << "[advanced] calendar 2025y/June/16d ok\n";

    // hh_mm_ss 拆分
    [[maybe_unused]] const hh_mm_ss hms{total};
    assert(hms.hours() == 2h);
    assert(hms.minutes() == 30min);

    // 时区(依赖系统 tzdb; 失败则跳过断言)
    try {
        const auto* zone = current_zone();
        if (zone != nullptr) {
            const zoned_time local{zone, system_clock::now()};
            std::cout << "[expert] current_zone=" << zone->name() << " local=" << local << '\n';
        }
    } catch (const std::exception& e) {
        std::cout << "[expert] tzdb unavailable: " << e.what() << '\n';
    }

    std::cout << "chrono_overview: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/chrono_overview", run>;

}  // namespace
