// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.3 char 与编码前缀
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : char_encoding_utf_prefixes
// Topic id : part3/section01/char_encoding_utf_prefixes
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/string_literal
//   https://en.cppreference.com/w/cpp/language/character_literal
//   https://en.cppreference.com/w/cpp/string/basic_string
//
// 要点: "" / u8"" / u"" / U"" / L""; char8_t/char16_t/char32_t/wchar_t。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>  // remove_cvref_t, is_same_v

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/char_encoding_utf_prefixes ===\n";

    // 普通字符串字面量: char 序列, 编码实现定义(现代工具链多为 UTF-8 源)
    [[maybe_unused]] const char* a = "abc";
    assert(a[0] == 'a');

    // UTF-8: char8_t (C++20); u8 前缀
    [[maybe_unused]] const char8_t* u8 = u8"UTF-8";
    assert(u8[0] == u8'U');
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(u8""[0])>, char8_t>);

    // UTF-16 码元
    [[maybe_unused]] const char16_t* u16 = u"UTF-16";
    assert(u16[0] == u'U');

    // UTF-32 码点
    [[maybe_unused]] const char32_t* u32 = U"UTF-32";
    assert(u32[0] == U'U');
    // 码点可直接写
    [[maybe_unused]] constexpr char32_t smiling = U'\U0001F600';  // 😀
    assert(smiling == 0x1F600);

    // 宽字符: 平台相关(Windows 常 16 位, *nix 常 32 位)
    [[maybe_unused]] const wchar_t* w = L"wide";
    assert(w[0] == L'w');
    std::cout << "[intro] sizeof(wchar_t)=" << sizeof(wchar_t) << '\n';

    // 对应 basic_string 别名
    std::u8string s8 = u8"hi";
    std::u16string s16 = u"hi";
    std::u32string s32 = U"hi";
    std::wstring ws = L"hi";
    assert(s8.size() == 2 && s16.size() == 2 && s32.size() == 2);
    assert(ws.size() == 2);

    // C++23 命名转义 / 定界转义(源文件与编译器支持时)
#if defined(__cpp_named_character_escapes) || (defined(__cplusplus) && __cplusplus >= 202302L)
    // "\N{...}" / "\u{1F600}" — 若编译器支持则编译通过
    // 用条件避免旧编译器炸; 这里用通用码点字面量演示等价意图
#endif
    [[maybe_unused]] constexpr char32_t alpha = U'\u03B1';  // Greek small letter alpha
    assert(alpha == 0x03B1);

    std::cout << "[advanced] u8/u/U/L prefixes and string aliases ok\n";
    std::cout << "[expert] wchar_t width is platform-defined; prefer "
                 "char8/16/32 for portable Unicode storage\n";
    std::cout << "char_encoding_utf_prefixes: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/char_encoding_utf_prefixes", run>;

}  // namespace
