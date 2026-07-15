// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.5 to_chars / from_chars
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : to_chars_from_chars
// Topic id : part3/section01/to_chars_from_chars
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/to_chars
//   https://en.cppreference.com/w/cpp/utility/from_chars
//
// 要点: 最快数值↔字符串; 无分配、无 locale、不抛异常; 返回 ptr+errc。

#include "learn/topic_registry.hpp"

#include <array>
#include <charconv>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/to_chars_from_chars ===\n";

    // to_chars: 数值 → 缓冲
    std::array<char, 32> buf{};
    auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), 42);
    assert(ec == std::errc{});
    const std::string_view out{buf.data(), static_cast<std::size_t>(ptr - buf.data())};
    assert(out == "42");
    std::cout << "[intro] to_chars(42) -> " << out << '\n';

    // 浮点
    auto [fptr, fec] = std::to_chars(buf.data(), buf.data() + buf.size(), 3.5);
    assert(fec == std::errc{});
    const std::string_view fout{buf.data(), static_cast<std::size_t>(fptr - buf.data())};
    assert(fout.find('3') != std::string_view::npos);
    std::cout << "[intro] to_chars(3.5) -> " << fout << '\n';

    // from_chars: 字符 → 数值; 可停在非数字处
    int value = 0;
    const char* str = "42abc";
    auto [p, e] = std::from_chars(str, str + 5, value);
    assert(e == std::errc{});
    assert(value == 42);
    assert(std::string_view{p} == "abc");
    std::cout << "[advanced] from_chars(\"42abc\") value=" << value << " rest=" << p << '\n';

    // 进制
    int hexv = 0;
    const char* hs = "FF";
    auto [hp, he] = std::from_chars(hs, hs + 2, hexv, 16);
    assert(he == std::errc{} && hexv == 255);
    (void)hp;

    // 错误: 空/非法
    int bad = -1;
    const char* zs = "xyz";
    auto [bp, be] = std::from_chars(zs, zs + 3, bad);
    assert(be == std::errc::invalid_argument);
    (void)bp;

    // 对比: stoi 抛异常 + 受 locale; to_string 分配
    assert(std::stoi("42") == 42);
    assert(std::to_string(42) == "42");

    std::cout << "[expert] prefer charconv for hot paths / serializers\n";
    std::cout << "to_chars_from_chars: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/to_chars_from_chars", run>;

}  // namespace
