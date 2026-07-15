// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库4 · 4.5 <stdfloat> (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : extended_floating_types_cpp23
// Topic id : part3/section04/extended_floating_types_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/types/floating-point
//   Feature macros: __STDCPP_FLOAT16_T__ / FLOAT32_T / FLOAT64_T / ...
//
// 要点: 固定宽度浮点别名; 可用性看硬件/实现; 必须特性探测。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <limits>
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
    std::cout << "=== part3/section04/extended_floating_types_cpp23 ===\n";

    // 基线: 标准 float/double
    static_assert(std::numeric_limits<float>::is_iec559 || !std::numeric_limits<float>::is_iec559);
    std::cout << "[intro] float digits10=" << std::numeric_limits<float>::digits10
              << " double digits10=" << std::numeric_limits<double>::digits10 << '\n';

#if defined(__STDCPP_FLOAT32_T__)
    std::float32_t f32 = static_cast<std::float32_t>(1.5);
    assert(f32 > std::float32_t{0} && f32 < std::float32_t{2});
    std::cout << "[advanced] std::float32_t available\n";
#else
    std::cout << "[advanced] std::float32_t not provided\n";
#endif

#if defined(__STDCPP_FLOAT64_T__)
    std::float64_t f64 = static_cast<std::float64_t>(2.5);
    assert(f64 > std::float64_t{2} && f64 < std::float64_t{3});
    std::cout << "[advanced] std::float64_t available\n";
#else
    std::cout << "[advanced] std::float64_t not provided\n";
#endif

#if defined(__STDCPP_FLOAT16_T__)
    std::float16_t f16 = static_cast<std::float16_t>(1);
    (void)f16;
    std::cout << "[expert] std::float16_t available\n";
#else
    std::cout << "[expert] std::float16_t not provided on this toolchain\n";
#endif

#if defined(__STDCPP_BFLOAT16_T__)
    std::bfloat16_t bf = static_cast<std::bfloat16_t>(1);
    (void)bf;
    std::cout << "[expert] std::bfloat16_t available (ML-friendly)\n";
#else
    std::cout << "[expert] std::bfloat16_t not provided\n";
#endif

    std::cout << "extended_floating_types_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/extended_floating_types_cpp23", run>;

}  // namespace
