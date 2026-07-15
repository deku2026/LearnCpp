// LearnCpp topic
// Doc      : 第6部分-支线G · G8.4 char/byte 检视
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : byte_char_observation
// Topic id : part6/g/section04/byte_char_observation
//
// 要点: char / unsigned char / std::byte 可别名任何对象以检视表示。
// 参考: [basic.lval] aliasing exceptions

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>

namespace {

std::size_t count_nonzero_bytes(std::span<const std::byte> bytes) {
    std::size_t n = 0;
    for (auto b : bytes) {
        if (b != std::byte{0}) {
            ++n;
        }
    }
    return n;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G8 byte/char observation of object representation ===\n";

    std::uint32_t x = 0x00ff00ffu;
    auto* p = reinterpret_cast<std::byte*>(&x);
    std::span<const std::byte> s{p, sizeof x};
    assert(count_nonzero_bytes(s) >= 2);

    // unsigned char 同样合法
    auto* uc = reinterpret_cast<unsigned char*>(&x);
    [[maybe_unused]] unsigned sum = 0;
    for (std::size_t i = 0; i < sizeof x; ++i) {
        sum += uc[i];
    }
    assert(sum == 0xff + 0xff);

    // 通过 char 写入再读回同一类型 — 需谨慎; 这里只读
    const char* cp = reinterpret_cast<const char*>(&x);
    (void)cp[0];

    std::cout << "  std::byte preferred for raw memory APIs (C++17)\n";
    std::cout << "byte_char_observation: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/byte_char_observation", run>;

}  // namespace
