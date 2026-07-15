// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.6 比较与 locale 陷阱
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : string_compare_and_locale_pitfalls
// Topic id : part3/section01/string_compare_and_locale_pitfalls
//
// Refs:
//   https://en.cppreference.com/w/cpp/string/basic_string/operator_cmp
//   https://en.cppreference.com/w/cpp/locale/locale
//   https://en.cppreference.com/w/cpp/string/byte/toupper
//
// 要点: string 比较=字节比较; toupper 不处理 Unicode; 可预测场景避开 locale。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cctype>
#include <iostream>
#include <locale>
#include <string>

namespace {

[[maybe_unused]] std::string ascii_upper(std::string s) {
    for (char& c : s) {
        if (c >= 'a' && c <= 'z') {
            c = static_cast<char>(c - 'a' + 'A');
        }
    }
    return s;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/string_compare_and_locale_pitfalls ===\n";

    // 字节字典序: 大写 ASCII 小于小写
    assert(std::string{"Zoo"} < std::string{"apple"});
    assert(std::string{"abc"} == std::string{"abc"});
    assert(std::string{"abc"} != std::string{"Abc"});

    // compare 返回 <0 / 0 / >0
    assert(std::string{"a"}.compare("b") < 0);

    std::cout << "[intro] byte-wise ordering: \"Zoo\" < \"apple\"\n";

    // locale-dependent ctype: 仅可靠处理单字节/实现定义集合
    const std::locale loc_c = std::locale::classic();
    using ctype_t = std::ctype<char>;
    [[maybe_unused]] const auto& ct = std::use_facet<ctype_t>(loc_c);
    assert(ct.toupper('a') == 'A');
    // 德文字母 ß / 多字节 UTF-8 不能靠 std::toupper 正确“大写化”
    const unsigned char e_acute_lead = 0xC3;  // UTF-8 é 首字节
    // toupper 对非 ASCII 字节行为无 Unicode 语义
    (void)std::toupper(static_cast<unsigned char>(e_acute_lead));

    // 可预测大小写: 自己限定 ASCII, 或用 ICU
    assert(ascii_upper("Hello") == "HELLO");
    std::cout << "[advanced] classic locale toupper('a')='A'; "
                 "use ICU for real Unicode case mapping\n";

    // 数值格式化与 locale: 某些环境下小数点为逗号 —— 跨平台序列化用 charconv
    std::cout << "[expert] avoid locale for wire formats; use charconv / "
                 "byte compares / ICU as needed\n";

    // 经典陷阱: 默认 locale 可能被全局 imbue 影响 iostream
    std::locale::global(std::locale::classic());
    assert(std::locale() == std::locale::classic());

    std::cout << "string_compare_and_locale_pitfalls: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/string_compare_and_locale_pitfalls", run>;

}  // namespace
