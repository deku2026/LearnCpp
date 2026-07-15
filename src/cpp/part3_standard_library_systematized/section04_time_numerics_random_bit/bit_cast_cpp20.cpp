// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库4 · 4.4 bit_cast
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : bit_cast_cpp20
// Topic id : part3/section04/bit_cast_cpp20
//
// Refs:
//   https://en.cppreference.com/w/cpp/numeric/bit_cast
//
// 要点: 类型安全位重解释; 取代别名 UB 的 reinterpret_cast; 要求同大小 trivially copyable。

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section04/bit_cast_cpp20 ===\n";

    const float f = 3.14f;
    const std::uint32_t bits = std::bit_cast<std::uint32_t>(f);
    const float back = std::bit_cast<float>(bits);
    assert(back == f);

    std::cout << "[intro] float 3.14f bits=0x" << std::hex << bits << std::dec << '\n';

    // 与 memcpy 等价语义, 但可 constexpr(C++20)
    std::uint32_t via_memcpy = 0;
    std::memcpy(&via_memcpy, &f, sizeof(f));
    assert(via_memcpy == bits);

    // 整数往返
    struct Pair {
        std::uint16_t a;
        std::uint16_t b;
    };
    const Pair p{0x1234, 0xABCD};
    const auto u = std::bit_cast<std::uint32_t>(p);
    const auto p2 = std::bit_cast<Pair>(u);
    assert(p2.a == p.a && p2.b == p.b);

    std::cout << "[advanced] bit_cast round-trip struct/uint32 ok\n";
    std::cout << "[expert] sizeof To==sizeof From; both trivially copyable\n";
    std::cout << "bit_cast_cpp20: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/bit_cast_cpp20", run>;

}  // namespace
