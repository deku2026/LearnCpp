// Topic    : #elifdef / #elifndef / #warning（C++23 预处理）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 9.1 / 9.2
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : elifdef_elifndef_warning_recap
// Topic id : part2/stage08/section04/elifdef_elifndef_warning_recap
// Refs     : https://en.cppreference.com/w/cpp/preprocessor/conditional
//            https://en.cppreference.com/w/cpp/preprocessor/error
//            提案 P2334 / P2437

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

// 用宏选择“后端”，演示 #ifdef / #elifdef / #elifndef 链。
// 默认走 software 路径，保证任意工具链下可链接运行。

#define LEARN_STAGE08_BACKEND_SOFTWARE 1

#if defined(LEARN_STAGE08_BACKEND_OPENGL)
inline const char* backend_name() {
    return "opengl";
}
#elifdef LEARN_STAGE08_BACKEND_VULKAN
inline const char* backend_name() {
    return "vulkan";
}
#elifdef LEARN_STAGE08_BACKEND_SOFTWARE
inline const char* backend_name() {
    return "software";
}
#elifndef LEARN_STAGE08_ALLOW_MISSING_BACKEND
inline const char* backend_name() {
    return "missing-disallowed";
}
#else
inline const char* backend_name() {
    return "fallback";
}
#endif

// #warning 会在编译期发出告警（不中断编译）。为避免污染默认 -Werror 构建，
// 仅在显式定义 LEARN_STAGE08_EMIT_WARNING 时启用。
#ifdef LEARN_STAGE08_EMIT_WARNING
#warning "LEARN_STAGE08_EMIT_WARNING is set: sample C++23 #warning"
#endif

// 对照：C++23 前多分支只能写 #elif defined(X)
// #ifdef A
// #elif defined(B)
// #endif

int feature_score() {
#if defined(LEARN_STAGE08_FEATURE_A)
    return 1;
#elifdef LEARN_STAGE08_FEATURE_B
    return 2;
#elifndef LEARN_STAGE08_FEATURE_OFF
    // FEATURE_OFF 未定义时走这里
    return 3;
#else
    return 0;
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [elifdef_elifndef_warning_recap] 入门：#elifdef 简写 ===\n";
    {
        // #elifdef X  ≡  #elif defined(X)
        // #elifndef X ≡  #elif !defined(X)
        const std::string be = backend_name();
        assert(be == "software");
        std::cout << "backend=" << be << '\n';
    }

    std::cout << "=== 进阶：#elifndef 与多分支后端选择 ===\n";
    {
        assert(feature_score() == 3);
        std::cout << "feature_score=" << feature_score() << " (#elifndef FEATURE_OFF path)\n";

        // 切换方式（编译命令加 -D）：
        // -DLEARN_STAGE08_BACKEND_VULKAN  → backend vulkan
        // 本运行默认 SOFTWARE。
        std::cout << "override with -DLEARN_STAGE08_BACKEND_VULKAN etc.\n";
    }

    std::cout << "=== 专家：#warning 标准化、与 #error、可移植性 ===\n";
    {
        // #warning "msg"：诊断为告警，翻译继续（P2437，来自实践既有扩展）。
        // #error "msg"：诊断为错误，翻译失败。
        //
        // 本文件默认不触发 #warning，以免 LEARNCPP_WERROR 构建变红。
        // 本地试验：-DLEARN_STAGE08_EMIT_WARNING 观察告警输出。
        //
        // 工具链：GCC 12+ / Clang 13+ 等对 #elifdef 支持较早；
        // 与“语句层 C++23”独立——这是预处理器特性。
        //
        // 工程：条件编译链一长就难测；优先 constexpr/策略类/链接期选择，
        // 宏只留给真正的平台分叉。

#ifndef LEARN_STAGE08_EMIT_WARNING
        std::cout << "#warning demo gated behind LEARN_STAGE08_EMIT_WARNING\n";
#endif
        std::cout << "C++23: #elifdef/#elifndef/#warning are standard\n";
        assert(std::string{backend_name()} == "software");
    }

    std::cout << "[elifdef_elifndef_warning_recap] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section04/elifdef_elifndef_warning_recap", run>;

}  // namespace
