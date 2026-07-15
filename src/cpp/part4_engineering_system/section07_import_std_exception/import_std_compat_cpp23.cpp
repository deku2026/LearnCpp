// LearnCpp topic
// Doc      : 第4部分-工程系统.md · import std 与头文件双轨兼容
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : import_std_compat_cpp23
// Topic id : part4/section07/import_std_compat_cpp23
//
// 要点: 过渡期同一逻辑可 #if 在 import std 与 #include 间切换;
//       勿在同一 TU 混用导致重复定义问题(实现相关)。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>
#include <version>

// 教学开关: 本仓库默认 0(头文件路径)
#ifndef LEARN_USE_IMPORT_STD
#define LEARN_USE_IMPORT_STD 0
#endif

#if LEARN_USE_IMPORT_STD
// import std;  // 实验开启时用
// import std.compat; // 若需全局 using 兼容
#endif

namespace {

// 两路径共用的业务函数(不依赖引入方式差异)
int sum_vec(const std::vector<int>& v) {
    return std::accumulate(v.begin(), v.end(), 0);
}

struct DualPathConfig {
    bool import_std;
    bool headers;
    bool valid() const {
        // 过渡策略: 二选一为主; 双开风险高
        return import_std != headers || (!import_std && headers);
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== import_std_compat_cpp23 ===\n";

    DualPathConfig cfg{
#if LEARN_USE_IMPORT_STD
        true, false
#else
        false, true
#endif
    };
    assert(cfg.valid());
    std::cout << "  path: " << (cfg.import_std ? "import std" : "headers") << '\n';

    assert(sum_vec({1, 2, 3, 4}) == 10);

    // 兼容层: std.compat 提供更接近 #include 的全局污染(实现定义)
    std::cout << "  std.compat = freestanding-ish globals bridge (when used)\n";

    // 宏门控模板
    // #if __cpp_lib_modules / 编译器自定义宏
    //   import std;
    // #else
    //   #include <vector>
    // #endif
    const bool gate_ready = false;  // 工程默认未开
    assert(!gate_ready || cfg.import_std);
    std::cout << "  set LEARN_USE_IMPORT_STD=1 only on known-good toolchains\n";

    std::cout << "import_std_compat_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section07/import_std_compat_cpp23", run>;

}  // namespace
