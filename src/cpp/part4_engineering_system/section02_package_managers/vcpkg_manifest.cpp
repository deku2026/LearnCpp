// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.2.1 vcpkg manifest 模式
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : vcpkg_manifest
// Topic id : part4/section02/vcpkg_manifest
//
// 要点: vcpkg.json 声明依赖 + builtin-baseline 锁目录基线;
//       CMAKE_TOOLCHAIN_FILE=.../vcpkg.cmake 一行接入。
//       忌 classic 全局装包导致 CI 缺失。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct Dep {
    std::string name;
    std::optional<std::string> version_ge;  // "version>="
};

struct Manifest {
    std::string name;
    std::string version;
    std::vector<Dep> dependencies;
    std::string builtin_baseline;  // commit hash
    std::string triplet;           // x64-windows / x64-linux-static ...
};

[[maybe_unused]] bool is_reproducible(const Manifest& m) {
    return !m.builtin_baseline.empty() && m.builtin_baseline.size() >= 7;
}

// 模拟 resolve: baseline + 名 → 钉住版本
struct Resolved {
    std::string name;
    std::string exact_version;
};

std::vector<Resolved> resolve(const Manifest& m, const std::map<std::string, std::string>& catalog) {
    std::vector<Resolved> out;
    for (const auto& d : m.dependencies) {
        auto it = catalog.find(d.name);
        assert(it != catalog.end());
        out.push_back({d.name, it->second});
    }
    return out;
}

// toolchain 路径约定
std::string toolchain_file(std::string_view vcpkg_root) {
    return std::string(vcpkg_root) + "/scripts/buildsystems/vcpkg.cmake";
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== vcpkg_manifest ===\n";

    // --- 入门: 文档示例形态 ---
    Manifest man{
        "myproject",
        "1.0.0",
        {{"fmt", std::nullopt},
         {"nlohmann-json", std::nullopt},
         {"spdlog", std::nullopt},
         {"boost-asio", std::string("1.84.0")}},
        "a1b2c3d4e5f6789012345678abcdef012345678",  // 假 baseline
        "x64-windows",
    };
    assert(is_reproducible(man));
    assert(man.dependencies.size() == 4);
    std::cout << "  project=" << man.name << " deps=" << man.dependencies.size() << " triplet=" << man.triplet << '\n';

    // 无 baseline → 不可重现
    Manifest bad = man;
    bad.builtin_baseline.clear();
    assert(!is_reproducible(bad));

    // --- 进阶: resolve ---
    std::map<std::string, std::string> catalog{
        {"fmt", "10.2.1"},
        {"nlohmann-json", "3.11.3"},
        {"spdlog", "1.13.0"},
        {"boost-asio", "1.84.0"},
    };
    auto r = resolve(man, catalog);
    assert(r.size() == 4);
    assert(r[0].exact_version == "10.2.1");

    // version>= 约束检查
    [[maybe_unused]] auto ge_ok = [](std::string_view have, std::string_view need) {
        return have >= need;  // 简化: 同位数字串比较对 1.84 够用
    };
    assert(ge_ok("1.84.0", "1.84.0"));

    // --- 专家: toolchain 接入 ---
    const auto tc = toolchain_file("$VCPKG_ROOT");
    assert(tc.find("vcpkg.cmake") != std::string::npos);
    std::cout << "  CMAKE_TOOLCHAIN_FILE=" << tc << '\n';

    // classic 模式坑: 全局 installed/ 不在仓库 → CI 没有
    [[maybe_unused]] const bool use_manifest_in_ci = true;
    assert(use_manifest_in_ci);
    std::cout << "  discipline: manifest+baseline in repo, not classic global\n";

    // CMake 侧: find_package(fmt CONFIG REQUIRED); fmt::fmt
    const std::string imported = "fmt::fmt";
    assert(imported.find("::") != std::string::npos);

    std::cout << "vcpkg_manifest: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section02/vcpkg_manifest", run>;

}  // namespace
