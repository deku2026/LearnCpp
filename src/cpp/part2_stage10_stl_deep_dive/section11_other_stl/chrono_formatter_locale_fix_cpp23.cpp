// Topic    : C++23 chrono formatter locale 处理修复
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.1(C++23 locale fix)
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : chrono_formatter_locale_fix_cpp23
// Topic id : part2/stage10/section11/chrono_formatter_locale_fix_cpp23
// Refs     : https://en.cppreference.com/w/cpp/chrono/system_clock/formatter
//            https://en.cppreference.com/w/cpp/utility/format/formatter
//            P2372 / LWG chrono format locale issues

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <format>
#include <iostream>
#include <locale>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    using namespace std::chrono;
    using namespace std::chrono_literals;
    std::cout << "=== [chrono_formatter_locale_fix_cpp23] format chrono ===\n";

    // ① duration 格式化: 默认输出 count + 单位后缀(实现/标准定义)
    {
        const std::string ms = std::format("{}", 150ms);
        assert(ms.find("150") != std::string::npos);
        std::cout << "  150ms → " << ms << '\n';

        const std::string sec = std::format("{}", 2s);
        assert(sec.find("2") != std::string::npos);
        std::cout << "  2s → " << sec << '\n';
    }

    // ② time_point / 日历类型格式说明符 %F %T
    {
        const sys_days day = 2025y / June / 16d;
        const auto tp = sys_time<seconds>{day} + 13h + 45min + 30s;
        const std::string date = std::format("{:%F}", tp);
        assert(date == "2025-06-16");
        const std::string time = std::format("{:%T}", tp);
        assert(time == "13:45:30");
        const std::string both = std::format("{:%F %T}", tp);
        assert(both == "2025-06-16 13:45:30");
        std::cout << "  sys_time: " << both << '\n';
    }

    // ③ year_month_day 直接 format
    {
        const year_month_day ymd = 2023y / March / 1d;
        const std::string s = std::format("{:%Y-%m-%d}", ymd);
        assert(s == "2023-03-01");
        std::cout << "  ymd: " << s << '\n';
    }

    // ④ locale 相关: format 可带 locale(C++20/23)
    // C++23 修了 chrono formatter 对 locale 的一致性(替代名/本地化说明符)
    {
        try {
            const std::locale loc = std::locale::classic();
            const sys_days day = 2020y / January / 15d;
            // L 前缀启用 locale 相关转换(若说明符需要)
            const std::string s = std::format(loc, "{:%Y-%m-%d}", year_month_day{day});
            assert(s == "2020-01-15");
            std::cout << "  locale classic date: " << s << '\n';
        } catch (const std::exception& ex) {
            std::cout << "  locale format path: " << ex.what() << '\n';
        }
    }

    // ⑤ weekday 名称(classic 下常为英文缩写, 依赖说明符)
    {
        const weekday wd = Thursday;
        assert(wd.ok());
        // %a 可能 locale 敏感; classic 下常见 "Thu"
        try {
            const std::string s = std::format("{:%a}", wd);
            assert(!s.empty());
            std::cout << "  Thursday %a → " << s << '\n';
        } catch (const std::exception& ex) {
            std::cout << "  weekday format: " << ex.what() << '\n';
        }
    }

    std::cout << "[chrono_formatter_locale_fix_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11/chrono_formatter_locale_fix_cpp23", run>;

}  // namespace
