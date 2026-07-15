// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : extended_floating_types_stdfloat_cpp23
// Topic id : part2/stage02/section01/extended_floating_types_stdfloat_cpp23
//
// Covers: C++23 <stdfloat> fixed-width floats, feature macros, fallbacks

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <limits>
#include <type_traits>
#include <version>

#if defined(__has_include)
#if __has_include(<stdfloat>)
#include <stdfloat>
#endif
#endif

namespace {

void demo_basics() {
    // Portable baseline: classic floating types always available.
    float f = 1.0f;
    double d = 2.0;
    long double ld = 3.0L;
    assert(f == 1.0f);
    assert(d == 2.0);
    assert(ld == 3.0L);
    static_assert(std::is_floating_point_v<float>);
    static_assert(std::is_floating_point_v<double>);
}

void demo_intermediate() {
#if defined(__STDCPP_FLOAT32_T__) && __STDCPP_FLOAT32_T__
    std::float32_t a = 1.0f32;
    assert(static_cast<float>(a) == 1.0f);
    static_assert(sizeof(std::float32_t) == 4);
#endif

#if defined(__STDCPP_FLOAT64_T__) && __STDCPP_FLOAT64_T__
    std::float64_t b = 2.0f64;
    assert(static_cast<double>(b) == 2.0);
    static_assert(sizeof(std::float64_t) == 8);
#endif

#if defined(__STDCPP_FLOAT16_T__) && __STDCPP_FLOAT16_T__
    std::float16_t h = 0.5f16;
    assert(static_cast<float>(h) == 0.5f);
#endif

    // Without extended types, classic types still demonstrate precision ideas.
    const float half = 0.5f;
    assert(half + half == 1.0f);
}

void demo_expert() {
#if defined(__STDCPP_BFLOAT16_T__) && __STDCPP_BFLOAT16_T__
    std::bfloat16_t bf = 1.5bf16;
    assert(static_cast<float>(bf) == 1.5f);
#endif

#if defined(__STDCPP_FLOAT128_T__) && __STDCPP_FLOAT128_T__
    std::float128_t q = 1.0f128;
    assert(static_cast<double>(q) == 1.0);
#endif

    // Feature-test awareness: extended floats are optional per platform.
#if defined(__cpp_lib_stdfloat)
    static_assert(__cpp_lib_stdfloat >= 202011L || __cpp_lib_stdfloat < 202011L);
#endif

    // IEC 559 classic floats remain the common portable path.
    if constexpr (std::numeric_limits<double>::is_iec559) {
        assert(std::numeric_limits<double>::digits == 53);
    }

    const double a = 0.1;
    const double b = 0.2;
    assert(std::abs((a + b) - 0.3) < 1e-12);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/extended_floating_types_stdfloat_cpp23", run>;

}  // namespace
