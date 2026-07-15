// LearnCpp topic
// Doc      : 第4部分-工程系统.md · import std 生态坑
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : import_std_ecosystem_caveats
// Topic id : part4/section07/import_std_ecosystem_caveats
//
// 要点: 实验 CMake 开关、生成器限制、与预编译头/unity/工具链成熟度;
//       第三方库大多仍头文件 → 混用成本。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct EcosystemCheck {
    std::string item;
    bool ready;
    std::string note;
};

[[maybe_unused]] int score(const std::vector<EcosystemCheck>& checks) {
    int ok = 0;
    for (const auto& c : checks) {
        if (c.ready) {
            ++ok;
        }
    }
    return ok;
}

// 本仓库实况(CMake 3.28 基线, 无 import std 实验默认)
std::vector<EcosystemCheck> learncpp_status() {
    return {
        {"cmake>=3.30", false, "repo requires 3.28; import std wants 3.30+"},
        {"CMAKE_EXPERIMENTAL_CXX_IMPORT_STD", false, "not enabled in presets"},
        {"generator Ninja", true, "CMakePresets base uses Ninja"},
        {"compiler std module BMI", false, "varies; treat experimental"},
        {"third_party header libs", true, "still #include based"},
        {"clang-tidy on modules", false, "tooling lag"},
        {"PCH + import std", false, "interaction fragile"},
    };
}

bool should_enable_in_production(const std::vector<EcosystemCheck>& c) {
    // 需要 cmake 与编译器 BMI 都 ready
    bool cmake = false, bmi = false;
    for (const auto& x : c) {
        if (x.item == "cmake>=3.30") {
            cmake = x.ready;
        }
        if (x.item == "compiler std module BMI") {
            bmi = x.ready;
        }
    }
    return cmake && bmi;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== import_std_ecosystem_caveats ===\n";

    auto st = learncpp_status();
    for (const auto& c : st) {
        std::cout << "  [" << (c.ready ? "x" : " ") << "] " << c.item << " — " << c.note << '\n';
    }
    assert(score(st) >= 1);  // 至少 Ninja ready
    assert(!should_enable_in_production(st));
    std::cout << "  production enable? " << (should_enable_in_production(st) ? "yes" : "no") << '\n';

    // 常见坑列表(可断言非空)
    std::vector<std::string> pitfalls{
        "mixing import std and heavy transitive headers unexpectedly",
        "BMI path not portable across compiler versions",
        "CI matrix must test modules job separately",
        "sccache/ccache keys more complex with BMI artifacts",
    };
    assert(pitfalls.size() >= 4);
    std::cout << "  pitfalls recorded=" << pitfalls.size() << '\n';

    // 与仓库模块路径对照
    std::cout << "  see also: section01/cmake_modules_and_import_std_cpp23\n";
    std::cout << "import_std_ecosystem_caveats: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section07/import_std_ecosystem_caveats", run>;

}  // namespace
