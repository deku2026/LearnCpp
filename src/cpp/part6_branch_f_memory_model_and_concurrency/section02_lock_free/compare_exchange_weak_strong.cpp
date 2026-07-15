// LearnCpp topic
// Doc      : 第6部分-支线F · F6.3 weak vs strong
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : compare_exchange_weak_strong
// Topic id : part6/f/section02/compare_exchange_weak_strong
//
// 要点: weak 允许伪失败 (spurious fail) → 循环里用更便宜;
//       strong 仅在值不等时失败 → 单次尝试用 strong。
// 参考: cppreference compare_exchange_weak

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F6 compare_exchange weak vs strong ===\n";

    std::atomic<int> a{10};

    // strong: 值匹配则必成功
    int expected = 10;
    [[maybe_unused]] bool ok = a.compare_exchange_strong(expected, 20);
    assert(ok);
    assert(a.load() == 20);

    // 失败: expected 被写成当前值
    expected = 10;
    ok = a.compare_exchange_strong(expected, 30);
    assert(!ok);
    assert(expected == 20);
    assert(a.load() == 20);

    // weak 在循环中
    expected = a.load();
    for (;;) {
        int next = expected + 1;
        if (a.compare_exchange_weak(expected, next, std::memory_order_acq_rel, std::memory_order_relaxed)) {
            break;
        }
    }
    assert(a.load() == 21);

    // 单次尝试: 用 strong 避免伪失败
    expected = 21;
    ok = a.compare_exchange_strong(expected, 21);
    assert(ok);  // 值不变的 CAS 也算成功

    // success/failure 内存序可不同
    expected = 21;
    a.compare_exchange_strong(expected, 22, std::memory_order_release, std::memory_order_relaxed);

    std::cout << "  loop → weak; one-shot → strong\n";
    std::cout << "  on failure, expected is updated to actual (both)\n";
    std::cout << "compare_exchange_weak_strong: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section02/compare_exchange_weak_strong", run>;

}  // namespace
