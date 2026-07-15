// LearnCpp topic
// Doc      : 第6部分-支线G · G7.3 / G8 strict aliasing
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : strict_aliasing_violation_deep
// Topic id : part6/g/section03/strict_aliasing_violation_deep
//
// 要点: 不得通过不相关类型指针读写同一对象; 用 bit_cast/memcpy/char。
// 不演示违规路径。

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace {

float int_bits_as_float(std::uint32_t u) {
    return std::bit_cast<float>(u);
}

std::uint32_t float_bits(float f) {
    return std::bit_cast<std::uint32_t>(f);
}

// 合法: 经 unsigned char / byte 检视
void write_bytes(unsigned char* p, std::size_t n, unsigned char v) {
    for (std::size_t i = 0; i < n; ++i) {
        p[i] = v;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G strict aliasing (safe type punning) ===\n";

    float f = 1.0f;
    auto bits = float_bits(f);
    assert(int_bits_as_float(bits) == 1.0f);

    // memcpy 类型双关
    int i = 0x01020304;
    unsigned char buf[sizeof(int)];
    std::memcpy(buf, &i, sizeof i);
    int j = 0;
    std::memcpy(&j, buf, sizeof j);
    assert(j == i);

    alignas(int) unsigned char storage[sizeof(int)]{};
    write_bytes(storage, sizeof storage, 0);
    std::memcpy(storage, &i, sizeof i);
    int k;
    std::memcpy(&k, storage, sizeof k);
    assert(k == i);

    // 危险 (不要): reinterpret_cast<int*>(&f); *p = ...
    std::cout << "  allowed: same type, similar types, char/byte, union active member\n";
    std::cout << "  C++20 bit_cast is the clean tool for value representation punning\n";
    std::cout << "strict_aliasing_violation_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/strict_aliasing_violation_deep", run>;

}  // namespace
