// Topic     : C++23 扩展浮点 <stdfloat>
// Doc       : 第2部分-阶段2 · 步骤 1.3
// cppreference: https://en.cppreference.com/cpp/header/stdfloat
//               https://en.cppreference.com/cpp/types/floating-point
//               https://en.cppreference.com/cpp/language/floating_literal
// 提案     : P1467R9
//
// 要点: std::floatN_t / bfloat16_t 提供可移植定宽 IEEE 浮点；支持可选；
//       与 float/double 不隐式互通；字面值后缀 f16/f32/f64/f128/bf16。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <type_traits>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<stdfloat>)
#include <stdfloat>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [extended_floating_types_stdfloat_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // 入门：为什么需要定宽浮点
    // -------------------------------------------------------------------------
    // float/double/long double 精度随平台变化（尤其 long double）。
    // 机器学习 / 跨平台数值需要“binary32 永远是 binary32”。
    std::cout << "[intro] classic sizes: float=" << sizeof(float) << " double=" << sizeof(double)
              << " long double=" << sizeof(long double) << '\n';

    // -------------------------------------------------------------------------
    // 进阶 / 专家：按特性宏探测并演示（不支持则跳过，不 fail）
    // -------------------------------------------------------------------------
#if defined(__has_include) && __has_include(<stdfloat>)

#if defined(__STDCPP_FLOAT32_T__) && (__STDCPP_FLOAT32_T__ == 1)
    {
        // 字面值后缀需与类型匹配；从 double 赋值可能需显式转换
        std::float32_t a = 1.5F32;
        std::float32_t b = static_cast<std::float32_t>(0.5);
        auto sum = static_cast<double>(a + b);
        assert(sum > 1.9 && sum < 2.1);
        std::cout << "[advanced] std::float32_t ok, a+b~=" << sum << '\n';
        // 与 float 不一定是同一类型
        std::cout << "[expert] is_same float32_t,float? " << std::boolalpha
                  << std::is_same_v<std::float32_t, float> << '\n';
    }
#else
    std::cout << "[advanced] __STDCPP_FLOAT32_T__ not available\n";
#endif

#if defined(__STDCPP_FLOAT64_T__) && (__STDCPP_FLOAT64_T__ == 1)
    {
        std::float64_t x = 0.1F64;
        assert(static_cast<double>(x) > 0.0);
        std::cout << "[advanced] std::float64_t ok, 0.1f64 as double=" << static_cast<double>(x) << '\n';
        std::cout << "[expert] is_same float64_t,double? " << std::boolalpha
                  << std::is_same_v<std::float64_t, double> << '\n';
    }
#else
    std::cout << "[advanced] __STDCPP_FLOAT64_T__ not available\n";
#endif

#if defined(__STDCPP_FLOAT16_T__) && (__STDCPP_FLOAT16_T__ == 1)
    {
        std::float16_t h = 1.0F16;
        assert(static_cast<float>(h) == 1.0f);
        std::cout << "[advanced] std::float16_t (binary16) available\n";
    }
#else
    std::cout << "[advanced] std::float16_t not available on this toolchain\n";
#endif

#if defined(__STDCPP_BFLOAT16_T__) && (__STDCPP_BFLOAT16_T__ == 1)
    {
        std::bfloat16_t bf = 1.5BF16;
        assert(static_cast<float>(bf) > 1.0f);
        std::cout << "[advanced] std::bfloat16_t available\n";
    }
#else
    std::cout << "[advanced] std::bfloat16_t not available on this toolchain\n";
#endif

#if defined(__STDCPP_FLOAT128_T__) && (__STDCPP_FLOAT128_T__ == 1)
    std::cout << "[advanced] std::float128_t available\n";
#else
    std::cout << "[advanced] std::float128_t not available on this toolchain\n";
#endif

    // 陷阱备忘：
    // - 扩展浮点之间 / 与经典浮点之间：不要假设有完整隐式转换层级
    // - 字面值后缀 f32 得到 float32_t，不是 float
    // - 库/iostream 对扩展浮点支持不齐，打印时常先 cast 到 double/float
    std::cout << "[expert] use feature macros; treat extended FP as optional aliases\n";

#else
    std::cout << "[intro] <stdfloat> header not present; skip demos\n";
    std::cout << "[expert] need recent libstdc++/libc++ + C++23; probe with __has_include\n";
#endif

    std::cout << "=== extended_floating_types_stdfloat_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/extended_floating_types_stdfloat_cpp23", run>;

}  // namespace
