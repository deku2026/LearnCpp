// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.1 TSan / 数据竞争
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : tsan_overview
// Topic id : part4/section05/tsan_overview
//
// 要点: 抓无同步并发访问。与 ASan 互斥, 须单独 CI 通道。
//       演示 mutex/atomic 正确同步; 不故意制造 data race。

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace {

bool tsan_built_in() {
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
    return true;
#endif
#endif
    return false;
}

[[maybe_unused]] int count_with_mutex(int n) {
    int counter = 0;
    std::mutex m;
    auto inc = [&] {
        for (int i = 0; i < n; ++i) {
            std::lock_guard<std::mutex> g(m);
            ++counter;
        }
    };
    std::thread a(inc), b(inc);
    a.join();
    b.join();
    return counter;
}

[[maybe_unused]] int count_with_atomic(int n) {
    std::atomic<int> counter{0};
    auto inc = [&] {
        for (int i = 0; i < n; ++i) {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
    };
    std::thread a(inc), b(inc);
    a.join();
    b.join();
    return counter.load();
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== tsan_overview ===\n";
    std::cout << "  tsan: " << (tsan_built_in() ? "ON" : "OFF") << '\n';

    constexpr int n = 50'000;
    assert(count_with_mutex(n) == 2 * n);
    assert(count_with_atomic(n) == 2 * n);
    std::cout << "  synchronized counters == " << (2 * n) << '\n';

    // 错误写法(勿运行无同步): ++counter 无锁 → TSan: data race
    std::cout << "  anti-pattern: unsync ++counter (TSan would report)\n";

    // 纪律: -fsanitize=thread 单独通道; 不可与 address 同开
    [[maybe_unused]] const bool can_mix_with_asan = false;
    assert(!can_mix_with_asan);
    std::cout << "  TSan lane is separate from ASan+UBSan\n";

    std::cout << "tsan_overview: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/tsan_overview", run>;

}  // namespace
