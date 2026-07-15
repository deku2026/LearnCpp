// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C9 false sharing)
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : false_sharing
// Topic id : part6/c/section05/false_sharing
//
// 要点: 两线程写同行不同变量 → 伪共享；alignas(destructive) 隔离。
// 无 data race：各线程写自己的 atomic 字段。
// 参考: hardware_destructive_interference_size

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <new>
#include <thread>

namespace {

struct FalseShared {
    std::atomic<long> a{0};
    std::atomic<long> b{0};  // 可能与 a 同一缓存行
};

struct alignas(std::hardware_destructive_interference_size) Padded {
    std::atomic<long> v{0};
};

struct NoFalseShare {
    Padded a;
    Padded b;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C9 false sharing ===\n";

    FalseShared fs;
    auto gap = reinterpret_cast<std::uintptr_t>(&fs.b) - reinterpret_cast<std::uintptr_t>(&fs.a);
    std::cout << "  FalseShared a/b gap=" << gap << " bytes\n";
    assert(gap < std::hardware_destructive_interference_size);

    NoFalseShare nfs;
    auto gap2 = reinterpret_cast<std::uintptr_t>(&nfs.b) - reinterpret_cast<std::uintptr_t>(&nfs.a);
    std::cout << "  NoFalseShare a/b gap=" << gap2 << " bytes\n";
    assert(gap2 >= std::hardware_destructive_interference_size);

    // 正确性：两线程写不同字段（padded）——无 data race
    constexpr int N = 100000;
    std::thread t1([&] {
        for (int i = 0; i < N; ++i) nfs.a.v.fetch_add(1, std::memory_order_relaxed);
    });
    std::thread t2([&] {
        for (int i = 0; i < N; ++i) nfs.b.v.fetch_add(1, std::memory_order_relaxed);
    });
    t1.join();
    t2.join();
    assert(nfs.a.v.load() == N);
    assert(nfs.b.v.load() == N);

    // 对照: FalseShared 在多核上可能更慢（伪共享）；单核难复现性能差
    std::thread u1([&] {
        for (int i = 0; i < N; ++i) fs.a.fetch_add(1, std::memory_order_relaxed);
    });
    std::thread u2([&] {
        for (int i = 0; i < N; ++i) fs.b.fetch_add(1, std::memory_order_relaxed);
    });
    u1.join();
    u2.join();
    assert(fs.a.load() == N && fs.b.load() == N);

    std::cout << "  fix: alignas(hardware_destructive_interference_size) per counter\n";
    std::cout << "  perf delta needs multi-core; correctness verified here\n";
    std::cout << "false_sharing: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/false_sharing", run>;

}  // namespace
