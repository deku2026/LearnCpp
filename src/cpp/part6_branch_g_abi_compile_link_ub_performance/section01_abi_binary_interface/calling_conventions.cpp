// LearnCpp topic
// Doc      : 第6部分-支线G · 调用约定
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : calling_conventions
// Topic id : part6/g/section01/calling_conventions
//
// 要点: 调用约定规定参数/返回值如何传递、谁清栈; 平台相关扩展。
// 参考: x64 ABI (SysV / Microsoft); x86 cdecl/stdcall

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>

namespace {

// 默认调用约定下的函数
[[maybe_unused]] int default_add(int a, int b) {
    return a + b;
}

// 多参数: 观察"前几个在寄存器"的 ABI 约定 (源码层只保证语义)
[[maybe_unused]] long long sum4(long long a, long long b, long long c, long long d) {
    return a + b + c + d;
}

// 结构体返回: 小结构可能通过寄存器
struct Pair {
    int x;
    int y;
};
Pair make_pair(int x, int y) {
    return {x, y};
}

// 大结构: 通常经隐藏指针返回
struct Big {
    char data[64];
};
Big make_big(char fill) {
    Big b{};
    for (char& c : b.data) {
        c = fill;
    }
    return b;
}

#if defined(_MSC_VER) && defined(_M_IX86)
// 仅 32-bit MSVC 常见
int __cdecl cdecl_add(int a, int b) {
    return a + b;
}
int __stdcall stdcall_add(int a, int b) {
    return a + b;
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G1 calling conventions ===\n";

    assert(default_add(1, 2) == 3);
    assert(sum4(1, 2, 3, 4) == 10);
    [[maybe_unused]] auto p = make_pair(5, 6);
    assert(p.x == 5 && p.y == 6);
    [[maybe_unused]] auto b = make_big('Z');
    assert(b.data[0] == 'Z' && b.data[63] == 'Z');

#if defined(_WIN64) || defined(_M_X64)
    std::cout << "  host Windows x64: RCX,RDX,R8,R9 / XMM for first args\n";
#elif defined(__x86_64__) && defined(__linux__)
    std::cout << "  host SysV AMD64: RDI,RSI,RDX,RCX,R8,R9\n";
#elif defined(__aarch64__)
    std::cout << "  host AArch64: X0-X7 integer args\n";
#else
    std::cout << "  host: see platform ABI doc for register assignment\n";
#endif

    std::cout << "  member functions: hidden this pointer\n";
    std::cout << "  mismatch of conventions across DLL boundary = hard bugs\n";
    std::cout << "calling_conventions: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/calling_conventions", run>;

}  // namespace
