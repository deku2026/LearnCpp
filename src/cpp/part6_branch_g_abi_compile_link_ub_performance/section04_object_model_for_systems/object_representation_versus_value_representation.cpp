// LearnCpp topic
// Doc      : 第6部分-支线G · G8.1 对象表示 vs 值表示
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : object_representation_versus_value_representation
// Topic id : part6/g/section04/object_representation_versus_value_representation
//
// 要点: 对象表示 = 占用的字节序列; 值表示 = 决定值的位。
//       padding 可在对象表示中但不参与值。
// 参考: [basic.types.general]

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

namespace {

struct Padded {
    char c;
    int i;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G8 object vs value representation ===\n";

    std::uint32_t x = 0x01020304u;
    unsigned char bytes[sizeof(x)];
    std::memcpy(bytes, &x, sizeof x);
    // 对象表示: 字节序相关
    std::cout << "  uint32 object bytes:";
    for (unsigned char b : bytes) {
        std::cout << ' ' << std::hex << static_cast<int>(b);
    }
    std::cout << std::dec << '\n';

    Padded p{};
    p.c = 'A';
    p.i = 7;
    std::cout << "  sizeof(Padded)=" << sizeof(Padded) << " (may include padding)\n";
    assert(sizeof(Padded) >= sizeof(char) + sizeof(int));

    // 比较值: 用成员, 不要 memcmp 含 padding 的结构当值相等
    [[maybe_unused]] Padded q = p;
    assert(q.c == p.c && q.i == p.i);

    // 值表示相同的 float 可能有不同对象表示 (NaN payload) — 教学提示
    float f = 1.0f;
    auto* fb = reinterpret_cast<unsigned char*>(&f);
    (void)fb;
    std::cout << "  padding bytes are part of object rep, not value semantics\n";
    std::cout << "object_representation_versus_value_representation: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/g/section04/object_representation_versus_value_representation", run>;

}  // namespace
