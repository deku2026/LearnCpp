// LearnCpp topic
// Doc      : 第6部分-支线F · F9.2 call_once
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : std_call_once
// Topic id : part6/f/section05/std_call_once
//
// 要点: std::call_once + once_flag 保证多线程只成功执行一次初始化。
// 参考: [thread.once] cppreference call_once

#include "learn/topic_registry.hpp"

#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F9 std::call_once ===\n";

    std::once_flag once;
    int init_count = 0;
    int value = 0;

    auto init = [&] {
        ++init_count;
        value = 123;
    };

    std::vector<std::jthread> ts;
    for (int i = 0; i < 10; ++i) {
        ts.emplace_back([&] { std::call_once(once, init); });
    }
    ts.clear();

    assert(init_count == 1);
    assert(value == 123);

    // 再次 call_once 不再执行
    std::call_once(once, init);
    assert(init_count == 1);

    // 异常: 若 init 抛错, 视为未完成, 其他线程可重试
    std::once_flag once2;
    int attempts = 0;
    try {
        std::call_once(once2, [&] {
            ++attempts;
            throw std::runtime_error("fail once");
        });
    } catch (const std::runtime_error&) {
    }
    std::call_once(once2, [&] {
        ++attempts;
        value = 9;
    });
    assert(attempts == 2);
    assert(value == 9);

    std::cout << "  good for lazy init of non-local resources\n";
    std::cout << "std_call_once: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/std_call_once", run>;

}  // namespace
