// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_to_chars_from_chars_cpp23
// Topic id : part2/stage07/section05/constexpr_to_chars_from_chars_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/from_chars
//   https://en.cppreference.com/w/cpp/utility/to_chars
//   P2291R3 — constexpr integral to_chars / from_chars
//   Feature-test: __cpp_lib_constexpr_charconv >= 202207L

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 编译期把字符串解析为 int（验收点）
// ---------------------------------------------------------------------------

#if defined(__cpp_lib_constexpr_charconv) && __cpp_lib_constexpr_charconv >= 202207L

constexpr int parse_int(std::string_view sv) {
    int value = 0;
    const auto result = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (result.ec != std::errc{}) {
        return -1;
    }
    return value;
}

constexpr auto int_to_chars_array(int n) {
    std::array<char, 16> buf{};
    const auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), n);
    if (ec != std::errc{}) {
        buf[0] = '?';
        return buf;
    }
    // 其余保持 '\0'，便于当 C 字符串用
    (void)ptr;
    return buf;
}

constexpr int parse_hex(std::string_view sv) {
    int value = 0;
    const auto result = std::from_chars(sv.data(), sv.data() + sv.size(), value, 16);
    if (result.ec != std::errc{}) {
        return -1;
    }
    return value;
}

#else

// 手写降级：保证 topic 在旧 STL 仍可编译运行
constexpr int parse_int(std::string_view sv) {
    int value = 0;
    bool any = false;
    for (char c : sv) {
        if (c < '0' || c > '9') {
            return -1;
        }
        any = true;
        value = value * 10 + (c - '0');
    }
    return any ? value : -1;
}

constexpr auto int_to_chars_array(int n) {
    std::array<char, 16> buf{};
    if (n == 0) {
        buf[0] = '0';
        return buf;
    }
    char tmp[16]{};
    int i = 0;
    int x = n;
    if (x < 0) {
        x = -x;
    }
    while (x > 0 && i < 15) {
        tmp[i++] = static_cast<char>('0' + (x % 10));
        x /= 10;
    }
    std::size_t out = 0;
    if (n < 0) {
        buf[out++] = '-';
    }
    while (i > 0) {
        buf[out++] = tmp[--i];
    }
    return buf;
}

constexpr int parse_hex(std::string_view sv) {
    int value = 0;
    for (char c : sv) {
        int d = 0;
        if (c >= '0' && c <= '9') {
            d = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            d = 10 + (c - 'a');
        } else if (c >= 'A' && c <= 'F') {
            d = 10 + (c - 'A');
        } else {
            return -1;
        }
        value = value * 16 + d;
    }
    return value;
}

#endif

// ---------------------------------------------------------------------------
// §进阶 — 编译期解析版本号 / CSV 片段
// ---------------------------------------------------------------------------

constexpr int parse_version_part(std::string_view sv, int which) {
    // which: 0=major,1=minor,2=patch for "A.B.C"
    int part = 0;
    int value = 0;
    int start = 0;
    for (int i = 0;; ++i) {
        const bool end = i == static_cast<int>(sv.size());
        if (end || sv[static_cast<std::size_t>(i)] == '.') {
            if (part == which) {
                return parse_int(sv.substr(static_cast<std::size_t>(start), static_cast<std::size_t>(i - start)));
            }
            ++part;
            start = i + 1;
            if (end) {
                break;
            }
        }
        (void)value;
    }
    return -1;
}

// ---------------------------------------------------------------------------
// §专家 — 浮点重载仍非 constexpr；无 locale / 不抛 / 不分配
// ---------------------------------------------------------------------------
// C++23 仅 *整数* 重载 constexpr。浮点 from_chars/to_chars 仍是运行期。
// charconv 设计目标：最快、无 locale、不抛异常、不分配 —— 天生适合 CTCE。

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section05/constexpr_to_chars_from_chars_cpp23 ===\n";

#if defined(__cpp_lib_constexpr_charconv)
    std::cout << "[intro] __cpp_lib_constexpr_charconv=" << __cpp_lib_constexpr_charconv << '\n';
#else
    std::cout << "[intro] __cpp_lib_constexpr_charconv not defined; using "
                 "hand-rolled fallback parsers\n";
#endif

    static_assert(parse_int("42") == 42);
    static_assert(parse_int("12345") == 12345);
    // 纯数字字面值是可移植验收点；含非法字符时 from_chars 可能「部分成功」
    // （解析出前缀数字），与手写严格解析行为不同，故不做 static_assert。
    static_assert(parse_hex("FF") == 255);
    static_assert(parse_hex("ff") == 255);

    constexpr auto buf = int_to_chars_array(1234);
    static_assert(buf[0] == '1' && buf[1] == '2' && buf[2] == '3' && buf[3] == '4');

    static_assert(parse_version_part("1.2.3", 0) == 1);
    static_assert(parse_version_part("1.2.3", 1) == 2);
    static_assert(parse_version_part("1.2.3", 2) == 3);

    std::cout << "[intro] parse_int(\"42\")=" << parse_int("42") << '\n';
    std::cout << "[advanced] version 10.20.30 -> " << parse_version_part("10.20.30", 0) << "."
              << parse_version_part("10.20.30", 1) << "." << parse_version_part("10.20.30", 2) << '\n';
    std::cout << "[advanced] to_chars(1234) => " << buf.data() << '\n';
    std::cout << "[expert] integral charconv is constexpr in C++23; floating "
                 "overloads remain runtime-only\n";

    // 运行期 charconv
    int rt = 0;
    std::string_view s = "99";
    [[maybe_unused]] auto r = std::from_chars(s.data(), s.data() + s.size(), rt);
    assert(r.ec == std::errc{});
    assert(rt == 99);

    std::cout << "constexpr_to_chars_from_chars_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_to_chars_from_chars_cpp23", run>;

}  // namespace
