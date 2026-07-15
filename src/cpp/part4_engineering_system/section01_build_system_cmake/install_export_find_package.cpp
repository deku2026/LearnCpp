// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.4 install / export / find_package
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : install_export_find_package
// Topic id : part4/section01/install_export_find_package
//
// 要点: install(TARGETS ... EXPORT) + Config.cmake 让下游 find_package 成功。
//       版本文件用 write_basic_package_version_file + SameMajorVersion 等策略。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace {

struct Version {
    int major{};
    int minor{};
    int patch{};
};

Version parse(std::string_view v) {
    Version out{};
    char dot = 0;
    std::string s{v};
    std::istringstream iss{s};
    iss >> out.major >> dot >> out.minor >> dot >> out.patch;
    return out;
}

bool same_major_compatible(Version req, Version have) {
    if (have.major != req.major) {
        return false;
    }
    // 同 major: have >= req
    if (have.minor != req.minor) {
        return have.minor > req.minor;
    }
    return have.patch >= req.patch;
}

[[maybe_unused]] bool exact_match(Version req, Version have) {
    return req.major == have.major && req.minor == have.minor && req.patch == have.patch;
}

// 模拟 find_package(Foo 1.2 CONFIG REQUIRED) 结果
struct FindResult {
    bool found;
    std::string config_path;
    Version version{};
};

FindResult find_package_config(std::string_view name, Version req,
                               const std::vector<std::pair<std::string, Version>>& installed) {
    for (const auto& [path, ver] : installed) {
        if (path.find(name) == std::string::npos) {
            continue;
        }
        if (same_major_compatible(req, ver)) {
            return {true, path, ver};
        }
    }
    return {false, {}, {}};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== install_export_find_package ===\n";

    // --- 入门: 版本解析 ---
    [[maybe_unused]] auto v = parse("1.4.2");
    assert(v.major == 1 && v.minor == 4 && v.patch == 2);

    // --- 进阶: SameMajorVersion ---
    assert(same_major_compatible(parse("1.2.0"), parse("1.4.2")));
    assert(!same_major_compatible(parse("1.2.0"), parse("2.0.0")));
    assert(!same_major_compatible(parse("1.5.0"), parse("1.4.9")));
    assert(exact_match(parse("1.4.2"), parse("1.4.2")));

    // 安装树里的 Config
    std::vector<std::pair<std::string, Version>> tree{
        {"lib/cmake/MyProject/MyProjectConfig.cmake", parse("1.0.0")},
        {"lib/cmake/fmt/fmt-config.cmake", parse("10.2.1")},
    };

    auto r = find_package_config("MyProject", parse("1.0.0"), tree);
    assert(r.found);
    assert(r.config_path.find("MyProjectConfig") != std::string::npos);
    std::cout << "  find_package MyProject -> " << r.config_path << '\n';

    auto missing = find_package_config("Boost", parse("1.84.0"), tree);
    assert(!missing.found);

    // --- 专家: 导出命名空间 MyProject::core ---
    // install(EXPORT ... NAMESPACE MyProject::)
    // 下游: target_link_libraries(app PRIVATE MyProject::core)
    const std::string exported_target = "MyProject::core";
    assert(exported_target.find("::") != std::string::npos);
    std::cout << "  imported target name=" << exported_target << '\n';

    // 文档骨架:
    // install(TARGETS core EXPORT coreTargets ...)
    // install(EXPORT coreTargets FILE coreConfig.cmake NAMESPACE MyProject:: ...)
    std::cout << "install_export_find_package: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/install_export_find_package", run>;

}  // namespace
