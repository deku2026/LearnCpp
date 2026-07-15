// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 多配置生成器 / CMAKE_BUILD_TYPE
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : multi_config
// Topic id : part4/section01/multi_config
//
// 要点: 单配置(Ninja/Makefiles)用 CMAKE_BUILD_TYPE;
//       多配置(VS/Xcode)用 --config 选择, 无单一 CMAKE_BUILD_TYPE。
// 仓库: CMakePresets.json 各 preset 设 CMAKE_BUILD_TYPE (Ninja 单配置)。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

enum class GeneratorKind { SingleConfig, MultiConfig };

struct BuildRequest {
    GeneratorKind kind;
    std::string configure_build_type;  // 仅 single 有效
    std::string build_config;          // multi: --config; single: 常等于 configure
};

// 解析最终生效配置
[[maybe_unused]] std::string effective_config(const BuildRequest& r) {
    if (r.kind == GeneratorKind::SingleConfig) {
        return r.configure_build_type.empty() ? "Debug" : r.configure_build_type;
    }
    // multi-config: 构建期选择
    return r.build_config.empty() ? "Debug" : r.build_config;
}

// NDEBUG 语义: Release 类配置通常定义 NDEBUG
bool expects_ndebug(std::string_view cfg) {
    return cfg == "Release" || cfg == "MinSizeRel" || cfg == "RelWithDebInfo";
}

// 本编译单元实际探测(教学: 与预设对照)
constexpr bool compiled_with_ndebug =
#ifdef NDEBUG
    true
#else
    false
#endif
    ;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== multi_config (single vs multi-config generators) ===\n";

    // --- 入门: Ninja 单配置 = 本仓库默认 ---
    BuildRequest ninja{
        GeneratorKind::SingleConfig,
        "Debug",  // CMakePresets windows-debug
        "",
    };
    assert(effective_config(ninja) == "Debug");
    assert(!expects_ndebug("Debug"));

    BuildRequest ninja_rel{GeneratorKind::SingleConfig, "Release", ""};
    assert(effective_config(ninja_rel) == "Release");
    assert(expects_ndebug("Release"));

    // --- 进阶: VS 多配置 ---
    BuildRequest vs{GeneratorKind::MultiConfig, /*ignored*/ "Debug", "Release"};
    assert(effective_config(vs) == "Release");
    // configure 时 CMAKE_BUILD_TYPE 常空; 用 --config Release 构建

    // --- 专家: 预设对照 ---
    // windows-debug  → Debug
    // windows-release → Release
    // windows-ci / linux-ci → RelWithDebInfo
    std::vector<std::pair<std::string, std::string>> presets{
        {"windows-debug", "Debug"},
        {"windows-release", "Release"},
        {"windows-ci", "RelWithDebInfo"},
        {"linux-ci", "RelWithDebInfo"},
    };
    for (const auto& [name, cfg] : presets) {
        BuildRequest r{GeneratorKind::SingleConfig, cfg, {}};
        assert(effective_config(r) == cfg);
        std::cout << "  preset " << name << " -> " << cfg << " NDEBUG?" << (expects_ndebug(cfg) ? "Y" : "N") << '\n';
    }

    std::cout << "  this TU compiled_with_ndebug=" << std::boolalpha << compiled_with_ndebug << '\n';
    // Debug 预设下通常 false; 不断言具体值(取决于你怎么编)

    // RelWithDebInfo: 优化 + 调试信息 — CI 常用
    assert(expects_ndebug("RelWithDebInfo"));
    assert(!expects_ndebug("Debug"));

    std::cout << "multi_config: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/multi_config", run>;

}  // namespace
