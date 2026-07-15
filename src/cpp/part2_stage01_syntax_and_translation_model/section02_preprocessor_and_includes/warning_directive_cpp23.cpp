// Topic     : #warning（C++23）
// Doc       : 第2部分-阶段1 · 步骤 3.5
// cppreference: https://en.cppreference.com/cpp/preprocessor/error
// 提案      : https://wg21.link/P2334
//
// 要点: #warning 发诊断但不中止编译；与 #error 对称；昔日是编译器扩展；
//       本 topic 默认不触发真实警告，以免污染 learn_cpp 日常构建。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

// #error：条件失败时硬停止（真实启用示例——仅在明显错误配置时）
#if defined(LEARNCPP_FORCE_STAGE01_ERROR_DEMO)
#error "LEARNCPP_FORCE_STAGE01_ERROR_DEMO is set: intentional #error demo"
#endif

// #warning：C++23 标准化。长期是 GCC/Clang 扩展。
// 默认放在 #if 0 中，避免每次编译刷屏；把 0 改 1 即可在本文件上看到告警。
#if 0
#warning "stage01 demo: #warning is standardized in C++23 (P2334)"
#endif

// 可移植"软提示"模式：没有标准 #warning 时退化为空，或用 pragma message（实现定义）
#if defined(__GNUC__) || defined(__clang__)
#define LEARN_PRAGMA_STRINGIZE0(x) #x
#define LEARN_PRAGMA_STRINGIZE(x) LEARN_PRAGMA_STRINGIZE0(x)
// 不在此默认发出 message；仅展示形态:
// #pragma message("file " LEARN_PRAGMA_STRINGIZE(__FILE__))
#endif

namespace {

enum class DiagKind { Warning, Error, None };

constexpr const char* kind_name(DiagKind k) {
    switch (k) {
        case DiagKind::Warning:
            return "warning";
        case DiagKind::Error:
            return "error";
        case DiagKind::None:
            return "none";
    }
    return "?";
}

// 教学用：描述何时选 #warning vs #error
[[maybe_unused]] DiagKind recommend(bool broken_config, bool deprecated_path) {
    if (broken_config) {
        return DiagKind::Error;  // 无法继续：#error
    }
    if (deprecated_path) {
        return DiagKind::Warning;  // 仍可编译：#warning
    }
    return DiagKind::None;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [warning_directive_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：与 #error 对照
    // -------------------------------------------------------------------------
    // #error "msg"    → 必须失败的配置（例如要求 C++23 却用了旧标准）
    // #warning "msg"  → 提醒废弃 API / 临时绕过 / 即将删除的路径
    assert(recommend(true, false) == DiagKind::Error);
    assert(recommend(false, true) == DiagKind::Warning);
    assert(recommend(false, false) == DiagKind::None);
    std::cout << "[intro] broken_config → #" << kind_name(DiagKind::Error) << ", deprecated → #"
              << kind_name(DiagKind::Warning) << '\n';

    // -------------------------------------------------------------------------
    // §进阶：历史与行为
    // -------------------------------------------------------------------------
    // · C++23 前: #warning 是常见扩展；严格 -pedantic 旧模式可能告警"未知指令"。
    // · C++23 起: 标准指令，语义为产生一条告警诊断。
    // · 若工程开启 -Werror / /WX，#warning 是否升格为错误取决于实现与选项
    //   （许多工具链中 warning 指令产生的是"普通警告"，会受 -Werror 影响）。
    // · 因此库作者用 #warning 提示废弃时，要意识到严苛 CI 可能失败——这有时是期望行为。
    std::cout << "[advanced] under -Werror, a #warning may fail the build (toolchain-dependent)\n";

    // -------------------------------------------------------------------------
    // §专家：如何在本仓库观察真实 #warning
    // -------------------------------------------------------------------------
    // 1) 将本文件中 `#if 0` 改为 `#if 1`，重新编译该 TU:
    //      cmake --build --preset windows-debug --target learn_cpp
    // 2) 或独立:
    //      clang-cl /std:c++latest /W4 /c warning_demo.cpp
    // 3) 对照 #error:
    //      cl /std:c++latest /DLEARNCPP_FORCE_STAGE01_ERROR_DEMO /c this_file.cpp
    //
    // 不要把 #warning 当日志系统：它在编译期触发，不进运行时。
    // 运行时诊断请用 std::cerr / 日志库。

    const std::string recipe = "flip #if 0 to 1 above the #warning line, rebuild, read compiler diagnostic";
    assert(recipe.find("#warning") != std::string::npos);

    std::cout << "[expert] " << recipe << '\n';
    std::cout << "=== warning_directive_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/warning_directive_cpp23", run>;

}  // namespace
