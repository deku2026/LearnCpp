// Topic     : volatile 不是线程同步工具
// Doc       : 第2部分-阶段2 · 步骤 3.4
// cppreference: https://en.cppreference.com/cpp/language/cv
//               https://en.cppreference.com/cpp/atomic/atomic
//
// 要点: volatile 抑制“对该对象访问”的编译器优化（每次真读/写）；
//       不提供原子性、内存序、互斥；多线程请用 std::atomic / mutex。

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

// 模拟“内存映射寄存器”：编译器不得把反复读写合并掉
void touch_mmapped_style(volatile int& reg) {
    reg = 1;
    reg = 2;
    int x = reg;
    int y = reg;
    assert(x == 2 && y == 2);
    (void)x;
    (void)y;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [volatile_is_not_thread_sync] ===\n";

    // -------------------------------------------------------------------------
    // 入门：volatile 的合法语义
    // -------------------------------------------------------------------------
    volatile int reg = 0;
    touch_mmapped_style(reg);
    std::cout << "[intro] volatile: every access is a real side effect on that object\n";

    // 普通变量：编译器可把重复读优化成一次；volatile 则不可（对优化器而言）
    volatile int v = 3;
    int a = v;
    int b = v;
    assert(a == 3 && b == 3);

    // -------------------------------------------------------------------------
    // 进阶：为什么不能当线程同步
    // -------------------------------------------------------------------------
    // 1) 数据竞争：多线程无同步写同一非 atomic 对象 = UB（volatile 不改变这一点）
    // 2) 无 happens-before：看不到其它线程的写入顺序保证
    // 3) 无原子 RMW：v++ 仍可能是读-改-写多条指令

    // 正确：atomic
    std::atomic<int> counter{0};
    auto worker = [&]() {
        for (int i = 0; i < 1000; ++i) {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
    };
    std::thread t1(worker);
    std::thread t2(worker);
    t1.join();
    t2.join();
    assert(counter.load() == 2000);
    std::cout << "[advanced] std::atomic counter=" << counter.load() << " (volatile would not make ++ race-free)\n";

    // -------------------------------------------------------------------------
    // 专家：与 signal、setjmp、旧代码迁移
    // -------------------------------------------------------------------------
    // - 硬件寄存器 / 某些 signal handler 共享标志：传统上用 volatile sig_atomic_t
    // - C++ 并发：std::atomic（可 lock-free）或互斥
    // - volatile 与 atomic 组合 rare：atomic 已表达可见性需求
    // - 从 Java/C# 的 volatile 心智迁移：C++ volatile ≠ 那些语言的 volatile

    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    assert(!flag.test_and_set());
    flag.clear();

    // 演示：单线程下 volatile 读写仍然“正常”
    volatile int only_local = 0;
    only_local = 42;
    assert(only_local == 42);

    std::cout << "[expert] volatile ≠ inter-thread synchronization; use atomics/mutexes\n";
    std::cout << "=== volatile_is_not_thread_sync: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/volatile_is_not_thread_sync", run>;

}  // namespace
