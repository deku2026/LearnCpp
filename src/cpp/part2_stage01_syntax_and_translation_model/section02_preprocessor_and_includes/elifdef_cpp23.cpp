// Topic     : #elifdef（C++23）
// Doc       : 第2部分-阶段1 · 步骤 3.5
// cppreference: https://en.cppreference.com/cpp/preprocessor/conditional
// 提案      : https://wg21.link/P2334
//
// 要点: #elifdef ID ≡ #elif defined(ID)；补齐 #ifdef 在 elif 链上的对称简写；
//       老编译器兜底；与 #ifdef / #elif defined 对照。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

// 配置探测场景：模拟"后端选择"
#define LEARN_BACKEND_CPU 1
// #define LEARN_BACKEND_GPU 1   // 有意不定义
// #define LEARN_BACKEND_NPU 1

// C++23 写法（本工程锁 C++23；若工具链极老，见下方 portable 分支说明）
#if defined(LEARN_BACKEND_GPU)
inline constexpr const char* k_backend = "gpu";
#elifdef LEARN_BACKEND_NPU
inline constexpr const char* k_backend = "npu";
#elifdef LEARN_BACKEND_CPU
inline constexpr const char* k_backend = "cpu";
#else
inline constexpr const char* k_backend = "unknown";
#endif

// 完全等价的旧写法（可移植兜底模板）
#if defined(LEARN_BACKEND_GPU)
inline constexpr int k_backend_code_portable = 1;
#elif defined(LEARN_BACKEND_NPU)
inline constexpr int k_backend_code_portable = 2;
#elif defined(LEARN_BACKEND_CPU)
inline constexpr int k_backend_code_portable = 3;
#else
inline constexpr int k_backend_code_portable = 0;
#endif

// 特征：部分实现提供 __cpp_elifdef；没有也不妨碍 C++23 模式使用指令本身
#if defined(__cpp_elifdef)
inline constexpr long k_elifdef_ftm = __cpp_elifdef;
#else
inline constexpr long k_elifdef_ftm = 0;
#endif

namespace {

std::string describe_chain() {
    // 文档化展开语义，便于对照 -E 输出
    return std::string{"#if defined(GPU) / #elifdef NPU / #elifdef CPU / #else"};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [elifdef_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：对称简写
    // -------------------------------------------------------------------------
    // 已有: #ifdef X / #ifndef X
    // 昔日 elif 只能: #elif defined(X) / #elif !defined(X)
    // C++23: #elifdef X / #elifndef X
    assert(std::string{k_backend} == "cpu");
    assert(k_backend_code_portable == 3);
    std::cout << "[intro] selected backend=" << k_backend << " portable_code=" << k_backend_code_portable << '\n';

    // -------------------------------------------------------------------------
    // §进阶：链的求值顺序 + 互斥配置
    // -------------------------------------------------------------------------
    // 预处理器从上到下找第一个条件为真的分支，其余在阶段 4 丢弃。
    // 若同时定义 GPU 与 CPU，#elifdef 链仍只取第一个真分支（这里 GPU 优先）。
    assert(describe_chain().find("#elifdef") != std::string::npos);
    std::cout << "[advanced] chain: " << describe_chain() << '\n';

    // 被丢弃分支即使语法荒谬也不编译——与 #if 0 相同模型
#if defined(LEARN_BACKEND_GPU)
    static_assert(false, "GPU not defined in this demo");
#elifdef LEARN_BACKEND_CPU
    // 我们在这里
    static_assert(true);
#endif

    // -------------------------------------------------------------------------
    // §专家：落地纪律
    // -------------------------------------------------------------------------
    // · 目标 C++23 时直接用 #elifdef 提高可读性。
    // · 需支持更老 -std= 时: 一律写 #elif defined(X)，或维护两套宏包装。
    // · 标准鼓励实现把 #elifdef 回溯到旧模式作扩展，但不可假设。
    // · 错误拼写 #elifdefd 等会被当成未知指令 → 条件链错乱或硬错误。
    // · 与 #error 组合: 在 #else 里 #error "unsupported backend"。
    //
    // 验证命令:
    //   clang++ -std=c++23 -E elifdef_demo.cpp | findstr backend
    //   clang++ -std=c++17 ...  // 若不支持 #elifdef 应失败或误解析

    if (k_elifdef_ftm != 0) {
        std::cout << "[expert] __cpp_elifdef=" << k_elifdef_ftm << '\n';
    } else {
        std::cout << "[expert] __cpp_elifdef not provided; directive still used under C++23\n";
    }

    std::cout << "=== elifdef_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/elifdef_cpp23", run>;

}  // namespace
