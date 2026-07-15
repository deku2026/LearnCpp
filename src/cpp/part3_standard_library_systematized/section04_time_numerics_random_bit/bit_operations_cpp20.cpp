// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库4 · 4.4 <bit> 位操作
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : bit_operations_cpp20
// Topic id : part3/section04/bit_operations_cpp20
//
// Refs:
//   https://en.cppreference.com/w/cpp/header/bit
//   https://en.cppreference.com/w/cpp/numeric/popcount

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section04/bit_operations_cpp20 ===\n";

    constexpr std::uint32_t x = 0b0001'0100u;  // 20
    static_assert(std::popcount(x) == 2);
    static_assert(std::countr_zero(x) == 2);
    static_assert(std::countl_zero(x) == 27);
    static_assert(std::bit_width(x) == 5);
    static_assert(!std::has_single_bit(x));
    static_assert(std::has_single_bit(std::uint32_t{8}));
    static_assert(std::bit_ceil(x) == 32);
    static_assert(std::bit_floor(x) == 16);

    std::cout << "[intro] popcount=" << std::popcount(x) << " countl_zero=" << std::countl_zero(x)
              << " countr_zero=" << std::countr_zero(x) << '\n';
    std::cout << "[intro] bit_width=" << std::bit_width(x) << " bit_ceil=" << std::bit_ceil(x)
              << " bit_floor=" << std::bit_floor(x) << '\n';

    // rotl / rotr
    [[maybe_unused]] constexpr std::uint8_t r = 0b1001'0001u;
    assert(std::rotl(r, 1) == 0b0010'0011u);
    assert(std::rotr(r, 1) == 0b1100'1000u);

    // endian
    if constexpr (std::endian::native == std::endian::little) {
        std::cout << "[advanced] endian::native = little\n";
    } else if constexpr (std::endian::native == std::endian::big) {
        std::cout << "[advanced] endian::native = big\n";
    } else {
        std::cout << "[advanced] endian::native = mixed/other\n";
    }

    std::cout << "[expert] <bit> is type-safe, constexpr-friendly bit toolbox\n";
    std::cout << "bit_operations_cpp20: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/bit_operations_cpp20", run>;

}  // namespace
