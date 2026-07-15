// LearnCpp topic
// Doc      : 第6部分-支线F · F2.3 / F4 acquire-release
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : acquire_release_semantics
// Topic id : part6/f/section01/acquire_release_semantics
//
// 要点: release 之前的写在 acquire 读到该值后可见; 成对同步, 无全局总序。
// 参考: [atomics.order] Preshing blogs

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F2 acquire / release semantics ===\n";

    // 发布指针/数据
    std::vector<int> buffer;
    std::atomic<bool> published{false};

    std::jthread writer([&] {
        buffer = {1, 2, 3, 4};  // 非原子写
        published.store(true, std::memory_order_release);
    });
    std::jthread reader([&] {
        while (!published.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        assert(buffer.size() == 4);
        assert(buffer[0] == 1 && buffer[3] == 4);
    });
    writer = std::jthread{};
    reader = std::jthread{};

    // RMW 用 acq_rel
    std::atomic<int> gate{0};
    int shared = 0;
    std::jthread t1([&] {
        shared = 123;
        gate.fetch_add(1, std::memory_order_release);
    });
    std::jthread t2([&] {
        int expected = 0;
        // 自旋直到看到 1
        while (gate.load(std::memory_order_acquire) == 0) {
            std::this_thread::yield();
        }
        assert(shared == 123);
        gate.fetch_add(1, std::memory_order_acq_rel);
    });
    t1 = std::jthread{};
    t2 = std::jthread{};
    assert(gate.load() == 2);

    // 错误配对示意 (不运行错误路径): acquire 必须读到 release 写入的值
    std::cout << "  loads: acquire or relaxed; stores: release or relaxed\n";
    std::cout << "  RMW that syncs both ways: acq_rel\n";
    std::cout << "acquire_release_semantics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/acquire_release_semantics", run>;

}  // namespace
