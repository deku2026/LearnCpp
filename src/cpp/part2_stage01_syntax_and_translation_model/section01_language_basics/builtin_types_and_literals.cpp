// Topic     : 基本内置类型与字面值
// Doc       : 第2部分-阶段1 · 步骤 6
// cppreference: https://en.cppreference.com/cpp/language/types
//               https://en.cppreference.com/cpp/language/integer_literal
//               https://en.cppreference.com/cpp/language/nullptr
//               https://en.cppreference.com/cpp/types/integer
//
// 要点: 基本类型目录；大小实现定义；cstdint 定宽；char 三型；字面值后缀；
//       nullptr vs NULL/0；未初始化局部 = 读即 UB；C++23 0uz / <stdfloat> 探测。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <climits>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<stdfloat>)
#include <stdfloat>
#endif
#endif

namespace {

void f_int(int) {}
void f_ptr(char*) {}

// nullptr 选中指针重载；字面 0 在重载决议里更偏 int（演示用独立重载集）
int overload_probe(int) {
    return 1;
}
int overload_probe(char*) {
    return 2;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [builtin_types_and_literals] ===\n";

    // -------------------------------------------------------------------------
    // §入门：算术类型 + 朴素初始化
    // -------------------------------------------------------------------------
    bool ok = true;
    char letter = 'A';
    int count = 42;
    double ratio = 3.14;
    void* raw = nullptr;
    (void)raw;

    assert(ok && letter == 'A' && count == 42);
    assert(ratio > 3.0 && ratio < 3.2);
    std::cout << "[intro] bool/char/int/double/void*/nullptr basics OK\n";

    // 大小：只保证 sizeof(char)==1 与宽度序，不保证 int==4
    static_assert(sizeof(char) == 1);
    static_assert(sizeof(char) <= sizeof(short));
    static_assert(sizeof(short) <= sizeof(int));
    static_assert(sizeof(int) <= sizeof(long));
    static_assert(sizeof(long) <= sizeof(long long));

    std::cout << "[intro] sizeof(int)=" << sizeof(int) << " sizeof(long)=" << sizeof(long) << " INT_MAX=" << INT_MAX
              << " limits::max=" << std::numeric_limits<int>::max() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：定宽整型、字面值、char 特殊性、nullptr
    // -------------------------------------------------------------------------
    std::int32_t i32 = -1;
    std::uint64_t u64 = 1;
    u64 <<= 40;
    assert(i32 == -1);
    assert(u64 == (std::uint64_t{1} << 40));

    // 字面值：进制、分隔符、后缀
    assert(0x2A == 42);
    assert(0b101010 == 42);        // C++14 二进制
    assert(052 == 42);             // 八进制前导 0 —— 易踩坑
    assert(1'000'000 == 1000000);  // C++14 分隔符
    auto u = 42u;
    auto ll = 42LL;
    static_assert(std::is_same_v<decltype(u), unsigned int>);
    static_assert(std::is_same_v<decltype(ll), long long>);

    // 浮点字面值默认 double；f → float
    static_assert(std::is_same_v<decltype(1.0), double>);
    static_assert(std::is_same_v<decltype(1.0f), float>);

    // 字符串字面量拼接（翻译阶段 6）与原始字符串
    const char* hi =
        "Hello, "
        "world";
    assert(std::string{hi} == "Hello, world");
    const char* winpath = R"(C:\Users\learn\file.txt)";
    assert(std::string_view{winpath}.find('\\') != std::string_view::npos);

    // char / signed char / unsigned char 是三个不同类型
    static_assert(!std::is_same_v<char, signed char>);
    static_assert(!std::is_same_v<char, unsigned char>);
    static_assert(!std::is_same_v<signed char, unsigned char>);

    // nullptr：类型 nullptr_t，不参与"当 int 用"
    static_assert(std::is_same_v<decltype(nullptr), std::nullptr_t>);
    assert(overload_probe(0) == 1);        // 选 int
    assert(overload_probe(nullptr) == 2);  // 选 char*
    // 旧 NULL 常是 0 的宏，重载时可能误入 int 版本——现代代码一律 nullptr。
    f_int(0);
    f_ptr(nullptr);

    std::cout << "[advanced] fixed-width, literals, nullptr overload OK\n";

    // -------------------------------------------------------------------------
    // §专家：未初始化、C++23 后缀、扩展浮点探测
    // -------------------------------------------------------------------------
    // 局部内置类型默认初始化 = 不初始化；读取是 UB。切勿演示真读未初始化值。
    int definite = 0;  // 声明即初始化
    int braced{};      // 值初始化为 0
    assert(definite == 0 && braced == 0);
    std::cout << "[expert] always initialize locals; reading indeterminate is UB\n";

    // C++23 size_t 字面值后缀 0uz —— 消除与 container::size() 的符号比较摩擦
#if defined(__cpp_size_t_suffix) && __cpp_size_t_suffix >= 202011L
    static_assert(std::is_same_v<decltype(0uz), std::size_t>);
    std::vector<int> v{1, 2, 3, 4};
    for (auto i = 0uz; i < v.size(); ++i) {
        assert(v[i] == static_cast<int>(i) + 1);
    }
    std::cout << "[expert] 0uz is std::size_t (C++23 size_t suffix)\n";
#else
    std::vector<int> v{1, 2, 3, 4};
    for (std::size_t i = 0; i < v.size(); ++i) {
        assert(v[i] == static_cast<int>(i) + 1);
    }
    std::cout << "[expert] __cpp_size_t_suffix unavailable; used size_t loop variable\n";
#endif

    // C++23 <stdfloat>：可选精确 IEEE 别名，落地前必须探测
#if defined(__has_include) && __has_include(<stdfloat>) && defined(__STDCPP_FLOAT32_T__)
    std::float32_t f32 = static_cast<std::float32_t>(1.5);
    assert(f32 > std::float32_t{0} && f32 < std::float32_t{2});
    std::cout << "[expert] std::float32_t available via <stdfloat>\n";
#else
    std::cout << "[expert] <stdfloat>/float32_t not available on this toolchain\n";
#endif

    // 对象表示窥视仅允许少数类型（unsigned char / std::byte 等）——阶段后文深讲
    const int sample = 0x01020304;
    unsigned char bytes[sizeof(sample)]{};
    std::memcpy(bytes, &sample, sizeof(sample));
    // 不断言字节序（实现定义），只断言拷贝完整
    assert(sizeof(bytes) == sizeof(int));
    std::cout << "[expert] endianness is implementation-defined; use memcpy for bytes\n";

    std::cout << "=== builtin_types_and_literals: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/builtin_types_and_literals", run>;

}  // namespace
