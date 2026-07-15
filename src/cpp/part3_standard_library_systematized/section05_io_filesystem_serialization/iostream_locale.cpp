// Topic    : iostream 与 locale（imbue / numpunct 探测）
// Doc      : 第3部分-标准库系统化.md · 库 5 IO + 本地化
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_locale
// Topic id : part3/section05/iostream_locale
// Refs     : https://en.cppreference.com/w/cpp/locale/locale
//            https://en.cppreference.com/w/cpp/io/basic_ios/imbue

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

namespace {

// 自定义 numpunct：把千分位改成 '_'，小数点改成 '#'
struct underscore_punct : std::numpunct<char> {
protected:
    char do_thousands_sep() const override { return '_'; }
    char do_decimal_point() const override { return '#'; }
    std::string do_grouping() const override { return "\3"; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [iostream_locale] classic \"C\" locale ===\n";
    {
        std::ostringstream oss;
        oss.imbue(std::locale::classic());
        oss << 1234567.5;
        // classic 通常无分组、小数点为 '.'
        const auto s = oss.str();
        std::cout << "classic: " << s << '\n';
        assert(s.find('.') != std::string::npos || s.find("1234567") != std::string::npos);
    }

    std::cout << "=== imbue 自定义 numpunct ===\n";
    {
        std::locale custom{std::locale::classic(), new underscore_punct};
        std::ostringstream oss;
        oss.imbue(custom);
        // showbase 无关；开启分组需要 showpos? 实际由 num_put 决定
        // 强制用 fixed 看小数点
        oss << std::fixed << 1234.5;
        const auto s = oss.str();
        std::cout << "custom numpunct: " << s << '\n';
        // 小数点应为 '#'
        assert(s.find('#') != std::string::npos);
    }

    std::cout << "=== 全局 locale 与流 locale 分离 ===\n";
    {
        // 流的 locale 默认拷贝自构造时的全局 locale；之后改全局不影响已有流
        const auto name = std::locale().name();
        std::cout << "global locale name: " << name << '\n';
        std::cout << "note: format/print use different locale paths; "
                     "iostream localization is facet-based\n";
    }

    std::cout << "=== 读侧：locale 影响解析（探测） ===\n";
    {
        std::istringstream iss("42");
        iss.imbue(std::locale::classic());
        int n = 0;
        assert(static_cast<bool>(iss >> n) && n == 42);
        std::cout << "parsed under classic: " << n << '\n';
    }

    std::cout << "[iostream_locale] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/iostream_locale", run>;

}  // namespace
