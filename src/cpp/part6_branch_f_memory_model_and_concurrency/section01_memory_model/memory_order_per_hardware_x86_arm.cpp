// LearnCpp topic
// Doc      : 第6部分-支线F · F3 x86/ARM 映射
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : memory_order_per_hardware_x86_arm
// Topic id : part6/f/section01/memory_order_per_hardware_x86_arm
//
// 要点: x86 强序 — acq/rel 常与 relaxed 同指令; seq_cst store 更贵。
//       ARM 弱序 — 需要 ldar/stlr 或 dmb。勿在 x86 上"碰巧正确"。
// 参考: Sutter atomic<> Weapons; godbolt

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F3 memory_order on x86 vs ARM ===\n";

    std::atomic<int> a{0};

    // 运行正确的发布 (与 ISA 无关的源码写法)
    int data = 0;
    std::atomic<bool> ready{false};
    std::jthread w([&] {
        data = 1;
        ready.store(true, std::memory_order_release);
    });
    std::jthread r([&] {
        while (!ready.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        assert(data == 1);
    });
    w = std::jthread{};
    r = std::jthread{};

    a.store(1, std::memory_order_relaxed);
    a.store(2, std::memory_order_release);
    a.store(3, std::memory_order_seq_cst);
    assert(a.load(std::memory_order_acquire) == 3);

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    std::cout << "  host: x86 family (typically strong)\n";
    std::cout << "    relaxed/acq/rel loads&stores often plain MOV\n";
    std::cout << "    seq_cst store may use XCHG/MFENCE\n";
#elif defined(__aarch64__) || defined(_M_ARM64)
    std::cout << "  host: AArch64 (weak)\n";
    std::cout << "    acquire load: LDAR; release store: STLR\n";
    std::cout << "    relaxed: plain LDR/STR\n";
#else
    std::cout << "  host: other ISA — check manual / godbolt\n";
#endif

    std::cout << "  portable code: pick order by C++ model, not by x86 luck\n";
    std::cout << "memory_order_per_hardware_x86_arm: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/memory_order_per_hardware_x86_arm", run>;

}  // namespace
