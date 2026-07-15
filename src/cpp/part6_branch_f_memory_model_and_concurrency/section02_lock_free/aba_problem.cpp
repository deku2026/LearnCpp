// LearnCpp topic
// Doc      : 第6部分-支线F · F7 ABA
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : aba_problem
// Topic id : part6/f/section02/aba_problem
//
// 要点: ABA = 值从 A→B→A, CAS 误判"没变"; 指针重用场景高发。
//       缓解: 标记指针/版本计数、hazard pointers、RCU、epoch。
// 参考: [atomic.types.operations] cppreference ABA

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cstdint>
#include <iostream>
#include <thread>

namespace {

// 带版本的指针: 低位存 tag (教学用整数槽)
struct Tagged {
    std::uint32_t index;
    std::uint32_t tag;
};

// 打包到 64-bit 原子
std::uint64_t pack(Tagged t) {
    return (static_cast<std::uint64_t>(t.tag) << 32) | t.index;
}
Tagged unpack(std::uint64_t v) {
    return {static_cast<std::uint32_t>(v & 0xffffffffu), static_cast<std::uint32_t>(v >> 32)};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F7 ABA problem ===\n";

    // 叙事演示 (安全, 不触发真实 UAF):
    // 线程1 读到 head=A; 线程2 pop A, push B, push A(重用);
    // 线程1 CAS(A, A.next_old) 可能成功但结构已坏。

    std::cout << "  classic: CAS sees pointer A again after A->B->A reuse\n";

    // 缓解: 版本号随每次修改递增
    std::atomic<std::uint64_t> head{pack({0, 0})};

    auto cas_push_slot = [&](std::uint32_t new_index) {
        for (;;) {
            auto raw = head.load(std::memory_order_relaxed);
            auto cur = unpack(raw);
            Tagged next{new_index, cur.tag + 1};
            if (head.compare_exchange_weak(raw, pack(next), std::memory_order_release, std::memory_order_relaxed)) {
                return;
            }
        }
    };

    cas_push_slot(1);
    cas_push_slot(2);
    {
        const auto h = unpack(head.load());
        assert(h.index == 2);
        assert(h.tag == 2);
    }

    // 错误 expected（旧 tag）的 CAS 必须失败——即便 index 想写回 2
    {
        std::uint64_t expected = pack({2, /*stale tag*/ 0});
        Tagged desired{2, 99};
        [[maybe_unused]] bool fail = head.compare_exchange_strong(expected, pack(desired));
        assert(!fail);
        // 失败后 expected 被写成当前值
        const auto cur = unpack(expected);
        assert(cur.index == 2 && cur.tag == 2);
    }
    // 正确 expected 才能成功，并推进 tag
    {
        std::uint64_t expected = pack({2, 2});
        Tagged good{1, 3};
        [[maybe_unused]] bool ok2 = head.compare_exchange_strong(expected, pack(good));
        assert(ok2);
        const auto h = unpack(head.load());
        assert(h.index == 1 && h.tag == 3);
    }

    std::cout << "  mitigations: tagged pointers, hazard pointers, RCU, GC\n";
    std::cout << "aba_problem: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section02/aba_problem", run>;

}  // namespace
