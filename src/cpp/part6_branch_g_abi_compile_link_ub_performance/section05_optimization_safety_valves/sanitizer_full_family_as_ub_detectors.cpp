// LearnCpp topic
// Doc      : 第6部分-支线G · G11.2 Sanitizer 全家桶
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : sanitizer_full_family_as_ub_detectors
// Topic id : part6/g/section05/sanitizer_full_family_as_ub_detectors
//
// 要点: ASan/UBSan/TSan/MSan/LSan 是 UB/并发/泄漏探测器;
//       本 topic 演示干净代码 + 说明各工具职责。
// 参考: clang.llvm.org/docs/AddressSanitizer.html 等

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G11 Sanitizer family as UB detectors ===\n";

    // ASan-friendly: 无 OOB/UAF
    {
        auto p = std::make_unique<int[]>(10);
        p[0] = 1;
        p[9] = 2;
        assert(p[0] + p[9] == 3);
    }

    // UBSan-friendly: 无溢出/空解引用
    int x = 100;
    [[maybe_unused]] int* px = &x;
    assert(*px == 100);

    // TSan-friendly: 同步访问
    {
        std::mutex m;
        int shared = 0;
        std::jthread t1([&] {
            std::lock_guard lock(m);
            ++shared;
        });
        std::jthread t2([&] {
            std::lock_guard lock(m);
            ++shared;
        });
        t1 = std::jthread{};
        t2 = std::jthread{};
        assert(shared == 2);
    }

    // MSan-friendly: 全初始化
    [[maybe_unused]] int y{};
    assert(y == 0);

    std::cout << "  ASan : address OOB, UAF, leaks(LSan)\n";
    std::cout << "  UBSan: UB subset (overflow, null, misalign, bool, ...)\n";
    std::cout << "  TSan : data races\n";
    std::cout << "  MSan : uninit reads (Clang; needs instrumented libs)\n";
    std::cout << "  typically one of ASan/TSan/MSan per build (not all at once)\n";
    std::cout << "  flags: -fsanitize=address,undefined,thread,memory\n";
    std::cout << "sanitizer_full_family_as_ub_detectors: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/sanitizer_full_family_as_ub_detectors", run>;

}  // namespace
