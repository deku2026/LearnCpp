// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库4 · 4.1 chrono format / locale (C++20/23)
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : chrono_formatter_locale_fix_cpp23
// Topic id : part3/section04/chrono_formatter_locale_fix_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/chrono/system_clock/formatter
//   https://en.cppreference.com/w/cpp/chrono/parse
//   LWG issues around chrono format locale (implementation-dependent fixes)
//
// 要点: format chrono 规格 {:%Y-%m-%d}; locale 影响部分转换说明符。

#include "learn/topic_registry.hpp"

#include <chrono>
#include <iostream>
#include <locale>
#include <string>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<format>)
#include <format>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    using namespace std::chrono;

    std::cout << "=== part3/section04/chrono_formatter_locale_fix_cpp23 ===\n";

    const year_month_day ymd = 2025y / May / 4d;
    assert(ymd.ok());

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // 日历 format
    const std::string date = std::format("{:%Y-%m-%d}", ymd);
    assert(date == "2025-05-04");
    std::cout << "[intro] format ymd -> " << date << '\n';

    // duration
    const auto d = 65s;
    const std::string ds = std::format("{}", d);
    // 典型 "65s"
    assert(ds.find('6') != std::string::npos);
    std::cout << "[intro] format duration -> " << ds << '\n';

    // time_point / zoned(若可用)
    const auto tp = sys_days{ymd} + 14h + 30min;
    const std::string tps = std::format("{:%Y-%m-%d %H:%M}", tp);
    assert(tps == "2025-05-04 14:30");
    std::cout << "[advanced] sys_time -> " << tps << '\n';

    // locale: classic 保证 C 数字
    std::locale::global(std::locale::classic());
    const std::string again = std::format(std::locale::classic(), "{:%Y}", ymd);
    assert(again == "2025");
    std::cout << "[expert] classic locale keeps numeric fields portable\n";
#else
    std::cout << "[intro] chrono format needs <format>\n";
#endif

    std::cout << "chrono_formatter_locale_fix_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/chrono_formatter_locale_fix_cpp23", run>;

}  // namespace
