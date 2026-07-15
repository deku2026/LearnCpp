// LearnCpp topic
// Doc      : 第6部分-支线G · G8.3 类型双关
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : type_punning_with_bit_cast_or_memcpy
// Topic id : part6/g/section04/type_punning_with_bit_cast_or_memcpy
//
// 要点: 合法类型双关 = bit_cast / memcpy; 联合体在 C++ 仅活动成员。
// 参考: P0476 bit_cast

#include "learn/topic_registry.hpp"

#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G8 type punning: bit_cast / memcpy ===\n";

    float f = 2.0f;
    std::uint32_t u = std::bit_cast<std::uint32_t>(f);
    [[maybe_unused]] float f2 = std::bit_cast<float>(u);
    assert(f2 == 2.0f);

    double d = -0.0;
    std::uint64_t bits = std::bit_cast<std::uint64_t>(d);
    // 符号位可能为 1 (平台 IEEE)
    (void)bits;

    // memcpy 等价于 bit_cast 对平凡类型
    std::uint32_t u2{};
    std::memcpy(&u2, &f, sizeof f);
    assert(u2 == u);

    // union 类型双关在 C++ 不是可移植的 (活动成员规则)
    // 不要写: union { int i; float f; } u; u.i=1; use u.f;

    std::cout << "  bit_cast requires same size + trivially copyable\n";
    std::cout << "type_punning_with_bit_cast_or_memcpy: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/type_punning_with_bit_cast_or_memcpy", run>;

}  // namespace
