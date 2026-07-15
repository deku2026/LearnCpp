// Topic    : 数据竞争是 UB —— mutex/atomic 安全对照
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : data_race_is_ub
// Topic id : part2/stage09/section08/data_race_is_ub
// Refs     : https://en.cppreference.com/w/cpp/language/ub
//            https://en.cppreference.com/w/cpp/atomic/atomic
//            ThreadSanitizer

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [data_race_is_ub] 主干：mutex 保护共享可变状态 ===\n";
    {
        std::mutex m;
        int counter = 0;
        auto worker = [&] {
            for (int i = 0; i < 1000; ++i) {
                std::lock_guard<std::mutex> lock(m);
                ++counter;
            }
        };
        std::thread t1(worker);
        std::thread t2(worker);
        t1.join();
        t2.join();
        assert(counter == 2000);
        std::cout << "mutex counter=" << counter << '\n';
    }

    std::cout << "=== 对抗：atomic 无锁计数 ===\n";
    {
        std::atomic<int> counter{0};
        auto worker = [&] {
            for (int i = 0; i < 1000; ++i) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        };
        std::thread t1(worker);
        std::thread t2(worker);
        t1.join();
        t2.join();
        assert(counter.load() == 2000);
        std::cout << "atomic counter=" << counter.load() << '\n';
    }

    std::cout << "=== 专节：数据竞争定义（不演示竞态）===\n";
    // 数据竞争：多线程对同一内存位置冲突访问（至少一写），且无 happens-before。
    // 数据竞争 = UB（不是“结果不确定这么简单”）。
    // 危险（勿运行教学演示）：
    //   int x=0; thread a{[&]{ ++x; }}; thread b{[&]{ ++x; }}; // UB
    // 护栏：TSan（-fsanitize=thread），且勿与 ASan 同开。
    std::cout << "data race is UB; use mutex/atomic; TSan in CI\n";

    std::cout << "[data_race_is_ub] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/data_race_is_ub", run>;

}  // namespace
