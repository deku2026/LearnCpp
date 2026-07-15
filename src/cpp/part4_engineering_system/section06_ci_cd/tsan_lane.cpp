// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.6.2 TSan 独立通道
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : tsan_lane
// Topic id : part4/section06/tsan_lane
//
// 要点: matrix sanitizer: [asan-ubsan, tsan]; TSan 单独 job。

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace {

struct CiJob {
    std::string name;
    std::string fsanitize;
};

bool conflicts(const CiJob& a, const CiJob& b) {
    // 同一进程 flags 不能 address+thread
    const bool a_addr = a.fsanitize.find("address") != std::string::npos;
    const bool b_thr = b.fsanitize.find("thread") != std::string::npos;
    const bool b_addr = b.fsanitize.find("address") != std::string::npos;
    const bool a_thr = a.fsanitize.find("thread") != std::string::npos;
    return (a_addr && b_thr) || (b_addr && a_thr);
}

int parallel_sum(int n) {
    std::atomic<int> s{0};
    auto work = [&] {
        for (int i = 0; i < n; ++i) {
            s.fetch_add(1, std::memory_order_relaxed);
        }
    };
    std::thread t1(work), t2(work);
    t1.join();
    t2.join();
    return s.load();
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== tsan_lane ===\n";

    CiJob asan_ubsan{"asan-ubsan", "address,undefined"};
    CiJob tsan{"tsan", "thread"};
    // 它们是不同 job, 不合并 flags
    assert(conflicts(asan_ubsan, tsan));  // 若错误合并会冲突
    std::cout << "  keep jobs separate: " << asan_ubsan.name << " | " << tsan.name << '\n';

    // 本 lane 旗标
    const std::string flags = "-fsanitize=thread -g";
    assert(flags.find("thread") != std::string::npos);
    assert(flags.find("address") == std::string::npos);

    assert(parallel_sum(20'000) == 40'000);
    std::cout << "  concurrent atomic path OK under TSan-minded code\n";

    // YAML 片段心智: matrix.sanitizer: [asan-ubsan, tsan]
    std::vector<std::string> matrix{"asan-ubsan", "tsan"};
    assert(matrix.size() == 2);
    std::cout << "tsan_lane: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/tsan_lane", run>;

}  // namespace
