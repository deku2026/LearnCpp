// LearnCpp topic
// Doc      : 第6部分-支线G · trap representation
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : trap_representation_note
// Topic id : part6/g/section04/trap_representation_note
//
// 要点: 某些类型可能存在"陷阱表示"(读即 UB); 现代 ILP32/LP64 上
//       普通 int 几乎没有; 仍勿读未初始化。
// 参考: C trap representation; C++ indeterminate values

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <iostream>
#include <limits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G trap representation (notes) ===\n";

    // 已初始化: 安全
    [[maybe_unused]] std::int32_t x = 0;
    assert(x == 0);

    // 所有位模式对 two's complement int 通常都是有效值
    std::uint32_t u = 0xffffffffu;
    [[maybe_unused]] std::int32_t as_signed = static_cast<std::int32_t>(u);
    assert(as_signed == -1);

    // 浮点 NaN 是有效值, 不是 trap (IEEE)
    [[maybe_unused]] double nan = std::numeric_limits<double>::quiet_NaN();
    assert(nan != nan);

    // bool 仅 0/1 是合法值表示; 其他位模式可能有问题 — 勿 bit_cast 乱造 bool
    [[maybe_unused]] bool ok = true;
    assert(ok);

    std::cout << "  historical CPUs had integer trap reps; rare on modern desktop\n";
    std::cout << "  C++: indeterminate value read is still UB for most scalars\n";
    std::cout << "  always initialize; use optional for empty state\n";
    std::cout << "trap_representation_note: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/trap_representation_note", run>;

}  // namespace
