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
// 要点: 类型安全位重解释; 取代别名 UB 的 reinterpret_cast;
//       要求同大小 + trivially copyable; C++20 可 constexpr。

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <type_traits>

namespace {

struct Pair {
    std::uint16_t a;
    std::uint16_t b;
};

// constexpr 往返(编译期)
constexpr std::uint32_t bits_of_one() {
    const float f = 1.0f;
    return std::bit_cast<std::uint32_t>(f);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section04/bit_cast_cpp20 ===\n";

    const float f = 3.14f;
    const std::uint32_t bits = std::bit_cast<std::uint32_t>(f);
    [[maybe_unused]] const float back = std::bit_cast<float>(bits);
    assert(back == f);
    static_assert(sizeof(float) == sizeof(std::uint32_t));
    static_assert(std::is_trivially_copyable_v<float>);

    std::cout << "[intro] float 3.14f bits=0x" << std::hex << bits << std::dec << '\n';

    // 与 memcpy 等价语义
    std::uint32_t via_memcpy = 0;
    std::memcpy(&via_memcpy, &f, sizeof(f));
    assert(via_memcpy == bits);

    // 整数/结构往返
    const Pair p{0x1234, 0xABCD};
    static_assert(sizeof(Pair) == sizeof(std::uint32_t));
    const auto u = std::bit_cast<std::uint32_t>(p);
    [[maybe_unused]] const auto p2 = std::bit_cast<Pair>(u);
    assert(p2.a == p.a && p2.b == p.b);

    // constexpr 路径
    constexpr auto one_bits = bits_of_one();
    static_assert(one_bits != 0);
    assert(std::bit_cast<float>(one_bits) == 1.0f);
    std::cout << "[advanced] constexpr bit_cast(1.0f)=0x" << std::hex << one_bits << std::dec << '\n';

    // 专家: endian 感知展示(不改变 bit_cast 语义)
    if constexpr (std::endian::native == std::endian::little) {
        std::cout << "[expert] native endian=little; bit_cast copies object representation\n";
    } else if constexpr (std::endian::native == std::endian::big) {
        std::cout << "[expert] native endian=big\n";
    } else {
        std::cout << "[expert] native endian=mixed/unknown\n";
    }
    // 错误: reinterpret_cast<uint32_t&>(f) 违反严格别名 → UB
    // 正确选择: bit_cast 或 memcpy
    std::cout << "[expert] do not type-pun via reinterpret_cast references\n";

    std::cout << "bit_cast_cpp20: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/bit_cast_cpp20", run>;

}  // namespace
