// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.6.2 TSan 独立通道
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : tsan_lane
// Topic id : part4/section06/tsan_lane
//
// 要点: matrix sanitizer: [asan-ubsan, tsan]; TSan 单独 job。
//       对照 4.5 混用纪律 + cmake/Sanitizers.cmake(仓库目前主 ASan/UBSan 开关)。

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
    std::string fsanitize;  // 编译/链接 flag 内容
    std::string os;         // ubuntu / windows ...
};

bool same_process_conflict(const CiJob& a, const CiJob& b) {
    // 同一进程 flags 不能 address+thread
    const bool a_addr = a.fsanitize.find("address") != std::string::npos;
    const bool b_thr = b.fsanitize.find("thread") != std::string::npos;
    const bool b_addr = b.fsanitize.find("address") != std::string::npos;
    const bool a_thr = a.fsanitize.find("thread") != std::string::npos;
    return (a_addr && b_thr) || (b_addr && a_thr);
}

// 正确: 两个 job, 各自一套 flags
bool matrix_valid(const std::vector<CiJob>& jobs) {
    // 不检查跨 job 冲突(它们是不同进程); 检查单 job 不自相矛盾
    for (const auto& j : jobs) {
        const bool addr = j.fsanitize.find("address") != std::string::npos;
        const bool thr = j.fsanitize.find("thread") != std::string::npos;
        if (addr && thr) {
            return false;
        }
    }
    return !jobs.empty();
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

    CiJob asan_ubsan{"asan-ubsan", "address,undefined", "ubuntu-24.04"};
    CiJob tsan{"tsan", "thread", "ubuntu-24.04"};
    // 若错误合并到同一 job 会冲突
    assert(same_process_conflict(asan_ubsan, tsan));
    std::cout << "  keep jobs separate: " << asan_ubsan.name << " | " << tsan.name << '\n';

    std::vector<CiJob> matrix{asan_ubsan, tsan};
    assert(matrix_valid(matrix));
    // 错误矩阵: 单 job 混装
    assert(!matrix_valid({{"bad", "address,thread", "ubuntu-24.04"}}));

    // 本 lane 旗标
    const std::string flags = "-fsanitize=thread -g";
    assert(flags.find("thread") != std::string::npos);
    assert(flags.find("address") == std::string::npos);

    // Windows MSVC/clang-cl: 文档 — 主推 ASan; TSan 走 Linux CI
    assert(tsan.os.find("ubuntu") != std::string::npos);
    std::cout << "  TSan primarily on Linux/GCC-Clang CI (doc 4.5.2)\n";

    assert(parallel_sum(20'000) == 40'000);
    std::cout << "  concurrent atomic path OK under TSan-minded code\n";

    // YAML 心智: matrix.sanitizer: [asan-ubsan, tsan]
    std::vector<std::string> names{"asan-ubsan", "tsan"};
    assert(names.size() == 2);
    // 仓库 Sanitizers.cmake 提供 LEARNCPP_ENABLE_ASAN / UBSAN;
    // TSan 作为独立 lane 文档化, 未必与 ASan 同 CMake 选项并存
    const std::string repo_asan = "LEARNCPP_ENABLE_ASAN";
    const std::string repo_ubsan = "LEARNCPP_ENABLE_UBSAN";
    assert(repo_asan.find("ASAN") != std::string::npos);
    assert(repo_ubsan.find("UBSAN") != std::string::npos);

    std::cout << "tsan_lane: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/tsan_lane", run>;

}  // namespace
