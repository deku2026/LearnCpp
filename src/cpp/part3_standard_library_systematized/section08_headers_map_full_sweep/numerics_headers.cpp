// Topic    : headers map · 数值 numerics
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : numerics_headers
// Topic id : part3/section08/numerics_headers
// Headers  : <numeric> <cmath> <complex> <valarray> <random>
//            <ratio> <numbers> <bit> <stdfloat>(C++23)

#include "learn/topic_registry.hpp"

#include <bit>
#include <cmath>
#include <complex>
#include <iostream>
#include <numeric>
#include <random>
#include <ratio>
#include <valarray>
#include <vector>

#if defined(__has_include)
#if __has_include(<numbers>)
#include <numbers>
#endif
#if __has_include(<stdfloat>)
#include <stdfloat>
#define LEARN_HAS_STDFLOAT 1
#endif
#endif
#ifndef LEARN_HAS_STDFLOAT
#define LEARN_HAS_STDFLOAT 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [numerics_headers] map probe ===\n";
    std::cout << "<numeric>  accumulate/reduce/gcd/lcm/iota\n";
    std::cout << "<cmath>    frexp/hypot/ceil...\n";
    std::cout << "<complex>  complex arithmetic\n";
    std::cout << "<valarray> historical array math\n";
    std::cout << "<random>   engines + distributions\n";
    std::cout << "<ratio>    compile-time rational\n";
    std::cout << "<numbers>  C++20 math constants\n";
    std::cout << "<bit>      bit ops (also under utility)\n";
    std::cout << "<stdfloat> C++23 extended float aliases\n";

    std::vector<int> v{1, 2, 3, 4};
    assert(std::accumulate(v.begin(), v.end(), 0) == 10);
    assert(std::gcd(12, 8) == 4);
    assert(std::lcm(4, 6) == 12);

    assert(std::abs(-3.5) == 3.5);
    std::complex<double> c{0, 1};
    assert(std::abs(c) == 1.0);
    std::cout << "complex i abs=" << std::abs(c) << '\n';

    std::valarray<int> va{1, 2, 3};
    va *= 2;
    assert(va[2] == 6);

    std::mt19937 gen{42};
    std::uniform_int_distribution<int> dist(1, 6);
    const int roll = dist(gen);
    assert(roll >= 1 && roll <= 6);
    std::cout << "mt19937 roll=" << roll << '\n';

    using half = std::ratio<1, 2>;
    static_assert(half::num == 1 && half::den == 2);

#if defined(__cpp_lib_math_constants)
    std::cout << "std::numbers::pi=" << std::numbers::pi << '\n';
#endif

    assert(std::has_single_bit(8u));
#if LEARN_HAS_STDFLOAT
    std::cout << "<stdfloat> present (extended float types)\n";
#else
    std::cout << "<stdfloat> not present on this toolchain\n";
#endif

    std::cout << "[numerics_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/numerics_headers", run>;

}  // namespace
