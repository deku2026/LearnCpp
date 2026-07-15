// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.5 constexpr charconv (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : constexpr_to_chars_from_chars_cpp23
// Topic id : part3/section01/constexpr_to_chars_from_chars_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/from_chars
//   P2291R3 — constexpr integral to_chars/from_chars
//   Feature-test: __cpp_lib_constexpr_charconv >= 202207L
//
// 要点: C++23 整数重载可 constexpr; 浮点仍运行期。

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

#if defined(__cpp_lib_constexpr_charconv) && __cpp_lib_constexpr_charconv >= 202207L

constexpr int parse_int(std::string_view sv) {
    int value = 0;
    const auto r = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (r.ec != std::errc{}) {
        return -1;
    }
    return value;
}

constexpr auto int_to_buf(int n) {
    std::array<char, 16> buf{};
    const auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), n);
    if (ec != std::errc{}) {
        buf[0] = '?';
        return buf;
    }
    (void)ptr;
    return buf;
}

#else

constexpr int parse_int(std::string_view sv) {
    int value = 0;
    if (sv.empty()) {
        return -1;
    }
    for (char c : sv) {
        if (c < '0' || c > '9') {
            return -1;
        }
        value = value * 10 + (c - '0');
    }
    return value;
}

constexpr auto int_to_buf(int n) {
    std::array<char, 16> buf{};
    if (n == 0) {
        buf[0] = '0';
        return buf;
    }
    char tmp[16]{};
    int i = 0;
    int x = n > 0 ? n : -n;
    while (x > 0 && i < 15) {
        tmp[i++] = static_cast<char>('0' + (x % 10));
        x /= 10;
    }
    std::size_t o = 0;
    if (n < 0) {
        buf[o++] = '-';
    }
    while (i > 0) {
        buf[o++] = tmp[--i];
    }
    return buf;
}

#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/constexpr_to_chars_from_chars_cpp23 ===\n";

#if defined(__cpp_lib_constexpr_charconv)
    std::cout << "[intro] __cpp_lib_constexpr_charconv=" << __cpp_lib_constexpr_charconv << '\n';
#else
    std::cout << "[intro] constexpr charconv FTM missing; hand-rolled fallback\n";
#endif

    static_assert(parse_int("42") == 42);
    static_assert(parse_int("0") == 0);
    constexpr auto buf = int_to_buf(1234);
    static_assert(buf[0] == '1' && buf[1] == '2' && buf[2] == '3' && buf[3] == '4');

    std::cout << "[advanced] parse_int(\"99\")=" << parse_int("99") << '\n';
    std::cout << "[advanced] int_to_buf(1234)=" << buf.data() << '\n';
    std::cout << "[expert] only integral overloads are constexpr in C++23\n";
    std::cout << "constexpr_to_chars_from_chars_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/constexpr_to_chars_from_chars_cpp23", run>;

}  // namespace
