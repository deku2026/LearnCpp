// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.7 CMakePresets.json
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : cmake_presets_json
// Topic id : part4/section01/cmake_presets_json
//
// 要点: Presets 固化 generator/binaryDir/cacheVariables, 替代记一堆 -D;
//       对照本仓库 CMakePresets.json + cmake/*.cmake 开关。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct ConfigurePreset {
    std::string name;
    std::string generator;
    std::string build_type;  // CMAKE_BUILD_TYPE for single-config
    bool werror = false;
    bool use_sccache = true;
    bool enable_ubsan = false;
    bool enable_asan = false;
};

// 镜像仓库 CMakePresets.json 中与教学相关的字段(手写对照, 非 JSON 解析)
std::vector<ConfigurePreset> learncpp_presets() {
    return {
        {"windows-debug", "Ninja", "Debug", false, true, false, false},
        {"windows-release", "Ninja", "Release", false, true, false, false},
        {"windows-ci", "Ninja", "RelWithDebInfo", true, true, false, false},
        {"linux-debug", "Ninja", "Debug", false, true, false, false},
        {"linux-ci", "Ninja", "RelWithDebInfo", true, true, true, false},
    };
}

const ConfigurePreset* find_preset(const std::vector<ConfigurePreset>& v, std::string_view name) {
    for (const auto& p : v) {
        if (p.name == name) {
            return &p;
        }
    }
    return nullptr;
}

// cmake/*.cmake 中的 cache 开关名(与 CMakeLists/Presets 一致)
struct RepoFlags {
    const char* sccache = "LEARNCPP_USE_SCCACHE";
    const char* werror = "LEARNCPP_WERROR";
    const char* asan = "LEARNCPP_ENABLE_ASAN";
    const char* ubsan = "LEARNCPP_ENABLE_UBSAN";
    const char* tidy = "LEARNCPP_ENABLE_CLANG_TIDY";
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== cmake_presets_json ===\n";

    const auto presets = learncpp_presets();
    assert(presets.size() >= 4);

    // --- 入门: cmake --preset windows-debug ---
    const auto* dbg = find_preset(presets, "windows-debug");
    assert(dbg);
    assert(dbg->generator == "Ninja");
    assert(dbg->build_type == "Debug");
    assert(!dbg->werror);
    std::cout << "  " << dbg->name << " -> " << dbg->generator << " / " << dbg->build_type << '\n';

    // --- 进阶: CI preset 开 -Werror; linux-ci 开 UBSan ---
    [[maybe_unused]] const auto* wci = find_preset(presets, "windows-ci");
    assert(wci && wci->werror && wci->build_type == "RelWithDebInfo");
    [[maybe_unused]] const auto* lci = find_preset(presets, "linux-ci");
    assert(lci && lci->enable_ubsan && lci->werror);
    std::cout << "  windows-ci WERROR; linux-ci WERROR+UBSAN\n";

    // base 隐藏预设: EXPORT_COMPILE_COMMANDS + sccache
    assert(dbg->use_sccache);
    RepoFlags flags{};
    assert(std::string_view{flags.sccache}.find("SCCACHE") != std::string_view::npos);
    assert(std::string_view{flags.ubsan}.find("UBSAN") != std::string_view::npos);
    std::cout << "  flags: " << flags.sccache << " / " << flags.werror << " / " << flags.asan << " / " << flags.ubsan
              << '\n';

    // --- 专家: 单配置 Ninja 需要 CMAKE_BUILD_TYPE; 命令形态 ---
    // cmake --preset windows-debug
    // cmake --build --preset windows-debug -j
    std::map<std::string, std::string> cache_like{
        {"CMAKE_BUILD_TYPE", dbg->build_type},
        {"CMAKE_EXPORT_COMPILE_COMMANDS", "ON"},
        {flags.sccache, dbg->use_sccache ? "ON" : "OFF"},
        {flags.werror, dbg->werror ? "ON" : "OFF"},
    };
    assert(cache_like["CMAKE_BUILD_TYPE"] == "Debug");
    assert(cache_like["CMAKE_EXPORT_COMPILE_COMMANDS"] == "ON");

    // binaryDir 心智: build/${presetName}
    const std::string binary_dir = std::string{"build/"} + dbg->name;
    assert(binary_dir == "build/windows-debug");
    std::cout << "  binaryDir pattern: " << binary_dir << '\n';

    // 文档 debug-asan 示例: ENABLE_ASAN ON — 本仓库用 LEARNCPP_ENABLE_ASAN
    ConfigurePreset asan_lane{"debug-asan", "Ninja", "Debug", false, true, false, true};
    assert(asan_lane.enable_asan && asan_lane.build_type == "Debug");
    std::cout << "  sanitizer presets: keep out of Release (see section05 matrix)\n";

    std::cout << "cmake_presets_json: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/cmake_presets_json", run>;

}  // namespace
