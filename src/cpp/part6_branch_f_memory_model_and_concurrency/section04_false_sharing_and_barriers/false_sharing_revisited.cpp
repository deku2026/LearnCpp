// LearnCpp topic
// Doc      : 第6部分-支线F · false sharing
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section04_false_sharing_and_barriers
// Item     : false_sharing_revisited
// Topic id : part6/f/section04/false_sharing_revisited
//
// 要点: 不同变量落在同一缓存行 → 写导致来回作废; 用 alignas 分到不同行。
// 参考: 支线 C false sharing; hardware_destructive_interference_size

#include "learn/topic_registry.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <new>
#include <thread>
#include <vector>

namespace {

struct Packed {
    std::atomic<int> a{0};
    std::atomic<int> b{0};
};

struct Padded {
#ifdef __cpp_lib_hardware_interference_size
    alignas(std::hardware_destructive_interference_size) std::atomic<int> a{0};
    alignas(std::hardware_destructive_interference_size) std::atomic<int> b{0};
#else
    alignas(64) std::atomic<int> a{0};
    alignas(64) std::atomic<int> b{0};
#endif
};

template <typename Pair>
long long bench_pair(Pair& p, int iters) {
    auto t0 = std::chrono::steady_clock::now();
    std::jthread t1([&] {
        for (int i = 0; i < iters; ++i) {
            p.a.fetch_add(1, std::memory_order_relaxed);
        }
    });
    std::jthread t2([&] {
        for (int i = 0; i < iters; ++i) {
            p.b.fetch_add(1, std::memory_order_relaxed);
        }
    });
    t1 = std::jthread{};
    t2 = std::jthread{};
    auto t1e = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t1e - t0).count();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F4 false sharing revisited ===\n";

    Packed packed;
    Padded padded;
    constexpr int kIters = 200000;

    auto us_packed = bench_pair(packed, kIters);
    auto us_padded = bench_pair(padded, kIters);

    assert(packed.a.load() == kIters && packed.b.load() == kIters);
    assert(padded.a.load() == kIters && padded.b.load() == kIters);

    std::cout << "  packed (likely false share) us=" << us_packed << '\n';
    std::cout << "  padded (separate lines)    us=" << us_padded << '\n';
    std::cout << "  (timing noisy on 1-core VMs; layout lesson still holds)\n";

    std::cout << "  sizeof(Packed)=" << sizeof(Packed) << " sizeof(Padded)=" << sizeof(Padded) << '\n';
    assert(sizeof(Padded) > sizeof(Packed));

    std::cout << "false_sharing_revisited: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section04/false_sharing_revisited", run>;

}  // namespace
