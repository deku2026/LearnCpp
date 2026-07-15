// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.5 Unity build
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : unity_build
// Topic id : part4/section01/unity_build
//
// 要点: UNITY_BUILD 把多个 .cpp 拼进同一 TU 编译, 减少重复解析。
//       风险: 匿名命名空间/static 符号/宏 跨文件泄漏 → ODR 隐患。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 模拟两个「源文件」若被 unity 合并后的符号可见性
struct TuSymbol {
    std::string file;
    std::string name;
    bool is_internal;  // static 或匿名命名空间
};

// 分文件编译: internal 不冲突
[[maybe_unused]] bool separate_ok(const std::vector<TuSymbol>& /*syms*/) {
    // 同名 internal 在不同文件 OK
    return true;
}

// Unity 合并后: 同名 internal 撞车
[[maybe_unused]] bool unity_conflict(const std::vector<TuSymbol>& syms) {
    for (std::size_t i = 0; i < syms.size(); ++i) {
        for (std::size_t j = i + 1; j < syms.size(); ++j) {
            if (syms[i].name == syms[j].name && syms[i].file != syms[j].file) {
                // 合并后都变成同一 TU 的内部符号 → 冲突/ODR 风险
                return true;
            }
        }
    }
    return false;
}

// 编译次数模型
int compile_invocations(int sources, int batch_size, bool unity) {
    if (!unity) {
        return sources;
    }
    return (sources + batch_size - 1) / batch_size;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== unity_build ===\n";

    // --- 入门: 批次数下降 ---
    constexpr int sources = 100;
    constexpr int batch = 8;  // CMAKE_UNITY_BUILD_BATCH_SIZE 一类
    assert(compile_invocations(sources, batch, false) == 100);
    assert(compile_invocations(sources, batch, true) == 13);
    std::cout << "  100 src -> unity batch8 => " << compile_invocations(sources, batch, true) << " compiles\n";

    // --- 进阶: 匿名命名空间同名 helper ---
    std::vector<TuSymbol> syms{
        {"a.cpp", "helper", true},
        {"b.cpp", "helper", true},  // 分文件 OK, unity 冲突
    };
    assert(separate_ok(syms));
    assert(unity_conflict(syms));
    std::cout << "  same internal name across files: unity_conflict=true\n";

    // 修复: 唯一化内部名, 或 UNITY_BUILD 排除该文件
    syms[1].name = "helper_b";
    assert(!unity_conflict(syms));

    // --- 专家: 宏污染 ---
    // 文件 A #define private public 调试hack 会污染同批 B —— 用示意标记
    [[maybe_unused]] bool macro_leak_risk = true;
    assert(macro_leak_risk);
    std::cout << "  pitfall: macros in one cpp leak into unity batch peers\n";

    // CMake:
    // set_target_properties(myapp PROPERTIES UNITY_BUILD ON)
    // set_source_files_properties(weird.cpp PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)
    [[maybe_unused]] const char* cmake_prop = "UNITY_BUILD";
    assert(std::string_view(cmake_prop) == "UNITY_BUILD");

    std::cout << "unity_build: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/unity_build", run>;

}  // namespace
