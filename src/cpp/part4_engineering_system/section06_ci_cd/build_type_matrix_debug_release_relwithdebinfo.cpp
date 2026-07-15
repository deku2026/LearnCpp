// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.6.1 多构建型
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : build_type_matrix_debug_release_relwithdebinfo
// Topic id : part4/section06/build_type_matrix_debug_release_relwithdebinfo
//
// 要点: Debug/Release/RelWithDebInfo 都要测 — 优化级不同会暴露不同 UB。
// 仓库 presets: windows-debug/release/ci(RelWithDebInfo)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct BuildTypeInfo {
    std::string name;
    int opt_level;  // 0..3 教学值
    bool has_debug_info;
    bool defines_ndebug;
};

BuildTypeInfo info(std::string_view name) {
    if (name == "Debug") {
        return {"Debug", 0, true, false};
    }
    if (name == "Release") {
        return {"Release", 3, false, true};
    }
    if (name == "RelWithDebInfo") {
        return {"RelWithDebInfo", 2, true, true};
    }
    if (name == "MinSizeRel") {
        return {"MinSizeRel", 2, false, true};
    }
    return {"Unknown", 0, false, false};
}

// 当前 TU 的 NDEBUG
constexpr bool this_tu_ndebug =
#ifdef NDEBUG
    true
#else
    false
#endif
    ;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== build_type_matrix_debug_release_relwithdebinfo ===\n";

    const std::vector<std::string> matrix{"Debug", "Release", "RelWithDebInfo"};
    for (const auto& n : matrix) {
        auto i = info(n);
        std::cout << "  " << i.name << " -O" << i.opt_level << " debug_info=" << std::boolalpha << i.has_debug_info
                  << " NDEBUG=" << i.defines_ndebug << '\n';
    }

    assert(info("Debug").opt_level < info("Release").opt_level);
    assert(info("RelWithDebInfo").has_debug_info);
    assert(info("RelWithDebInfo").defines_ndebug);
    assert(!info("Debug").defines_ndebug);

    // presets 映射
    assert(info("Debug").name == "Debug");                    // windows-debug
    assert(info("Release").name == "Release");                // windows-release
    assert(info("RelWithDebInfo").name == "RelWithDebInfo");  // *-ci

    std::cout << "  this TU NDEBUG=" << this_tu_ndebug << '\n';

    // 为何矩阵: Release 激进优化可能把 UB「变成」崩溃/错误结果
    std::cout << "  run tests on all types — UB may hide in Debug only\n";
    std::cout << "build_type_matrix_debug_release_relwithdebinfo: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/build_type_matrix_debug_release_relwithdebinfo", run>;

}  // namespace
