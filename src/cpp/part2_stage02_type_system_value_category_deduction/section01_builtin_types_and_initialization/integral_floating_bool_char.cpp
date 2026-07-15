// Topic     : 内置算术类型：整型 / 浮点 / bool / 字符家族
// Doc       : 第2部分-阶段2 · 步骤 1.1
// cppreference: https://en.cppreference.com/cpp/language/types
//               https://en.cppreference.com/cpp/language/integer_literal
//               https://en.cppreference.com/cpp/language/floating_literal
//               https://en.cppreference.com/cpp/language/character_literal
//
// 要点: sizeof 只保证下限与相对关系；char / signed char / unsigned char 是三种类型；
//       char 符号性实现定义；bool 与整型可互转但语义不同；浮点默认字面值是 double。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <climits>
#include <cstdint>
#include <iostream>
#include <limits>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [integral_floating_bool_char] ===\n";

    // -------------------------------------------------------------------------
    // 入门：基本族 + 字面值类型
    // -------------------------------------------------------------------------
    [[maybe_unused]] bool flag = true;
    [[maybe_unused]] char letter = 'Z';
    [[maybe_unused]] int count = 42;
    float f = 1.5f;
    double d = 1.5;  // 无后缀浮点字面值 → double
    long double ld = 1.5L;

    assert(flag);
    assert(letter == 'Z' && count == 42);
    static_assert(std::is_same_v<decltype(1.0), double>);
    static_assert(std::is_same_v<decltype(1.0f), float>);
    static_assert(std::is_same_v<decltype(1.0L), long double>);
    static_assert(std::is_same_v<decltype(42), int>);
    static_assert(std::is_same_v<decltype(42u), unsigned int>);
    static_assert(std::is_same_v<decltype(42LL), long long>);

    // 整型大小：只保证相对关系与下限，不保证 int==4
    static_assert(sizeof(char) == 1);
    static_assert(sizeof(char) <= sizeof(short));
    static_assert(sizeof(short) <= sizeof(int));
    static_assert(sizeof(int) <= sizeof(long));
    static_assert(sizeof(long) <= sizeof(long long));
    static_assert(sizeof(float) <= sizeof(double));
    static_assert(sizeof(double) <= sizeof(long double));

    std::cout << "[intro] sizeof(int)=" << sizeof(int) << " sizeof(long)=" << sizeof(long)
              << " sizeof(long double)=" << sizeof(long double) << " INT_MAX=" << INT_MAX << '\n';
    (void)f;
    (void)d;
    (void)ld;

    // -------------------------------------------------------------------------
    // 进阶：字符家族三兄弟 + bool 语义
    // -------------------------------------------------------------------------
    // char / signed char / unsigned char 是【三种不同】类型
    static_assert(!std::is_same_v<char, signed char>);
    static_assert(!std::is_same_v<char, unsigned char>);
    static_assert(!std::is_same_v<signed char, unsigned char>);

    // char 的符号性是实现定义的——当小整数用时显式写 signed/unsigned char
    const bool char_is_signed = std::numeric_limits<char>::is_signed;
    std::cout << "[advanced] char is_signed=" << std::boolalpha << char_is_signed << '\n';

    // 大值赋给 char：在 signed char 平台可能变负；用 unsigned char 语义确定
    [[maybe_unused]] unsigned char byte = 200;
    assert(static_cast<int>(byte) == 200);
    signed char sbyte = static_cast<signed char>(200);  // 实现定义转换结果常见为负
    (void)sbyte;

    // C++20 起还有 char8_t / 既有 char16_t / char32_t / wchar_t
    [[maybe_unused]] char8_t u8c = u8'A';
    [[maybe_unused]] char16_t u16c = u'A';
    [[maybe_unused]] char32_t u32c = U'A';
    [[maybe_unused]] wchar_t wc = L'A';
    assert(static_cast<char>(u8c) == 'A');
    assert(u16c == u'A' && u32c == U'A' && wc == L'A');
    static_assert(sizeof(char8_t) == 1);
    static_assert(sizeof(char16_t) == 2);
    static_assert(sizeof(char32_t) == 4);

    // bool：true/false；与整型转换 true→1 false→0；非 0 整型→true
    assert(static_cast<int>(true) == 1);
    assert(static_cast<int>(false) == 0);
    assert(static_cast<bool>(2) == true);
    assert(static_cast<bool>(0) == false);
    std::cout << "[advanced] char family + bool conversions OK\n";

    // -------------------------------------------------------------------------
    // 专家：实现定义边界 + 不要拿 char 当“字节小整数”默认可移植
    // -------------------------------------------------------------------------
    // long 在 Windows LP64/LLP64 差异：Win64 上 long 常 32 位，Linux x64 常 64 位
    std::cout << "[expert] long bits=" << (sizeof(long) * CHAR_BIT)
              << " long long bits=" << (sizeof(long long) * CHAR_BIT) << " — need fixed width? use <cstdint>\n";

    // 浮点：long double 精度平台相关（x86 可能 80-bit extended）
    std::cout << "[expert] numeric_limits<double>::digits10=" << std::numeric_limits<double>::digits10
              << " long double digits10=" << std::numeric_limits<long double>::digits10 << '\n';

    // 字符字面值类型：'A' 在 C++ 中是 int? 否——C++ 中普通字符字面值类型是 char
    static_assert(std::is_same_v<decltype('A'), char>);
    // 多字符字面值 'AB' 实现定义（类型 int），教学上应避免依赖
    static_assert(std::is_same_v<decltype(u8'A'), char8_t>);

    // 整数提升：short/char 参与算术时常提升为 int
    signed char a = 100;
    signed char b = 100;
    auto sum = a + b;  // 通常是 int，不是 signed char
    static_assert(std::is_same_v<decltype(sum), int>);
    assert(sum == 200);
    std::cout << "[expert] a+b promotes to int; sum=" << sum << '\n';

    std::cout << "=== integral_floating_bool_char: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/integral_floating_bool_char", run>;

}  // namespace
