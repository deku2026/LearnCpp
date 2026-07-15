// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.2.4 语义化版本与锁定
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : semver_and_lockfiles
// Topic id : part4/section02/semver_and_lockfiles
//
// 要点: MAJOR.MINOR.PATCH; major 破坏兼容。lockfile/baseline 锁整树确切版本。
//       vcpkg builtin-baseline; conan lock create。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace {

struct SemVer {
    int major{};
    int minor{};
    int patch{};
    auto operator<=>(const SemVer&) const = default;
};

SemVer parse_semver(std::string_view s) {
    SemVer v{};
    std::string tmp{s};
    char d1 = 0, d2 = 0;
    std::istringstream iss{tmp};
    iss >> v.major >> d1 >> v.minor >> d2 >> v.patch;
    return v;
}

// ^1.2.3 → >=1.2.3 <2.0.0 (简化 caret)
bool satisfies_caret(SemVer req, SemVer have) {
    if (have.major != req.major) {
        return false;
    }
    return have >= req;
}

// 兼容范围 version>=
bool satisfies_ge(SemVer req, SemVer have) {
    return have >= req;
}

struct Lockfile {
    std::map<std::string, SemVer> exact;  // 整树钉死
};

SemVer resolve_with_lock(const Lockfile& lock, std::string_view name, SemVer range_min) {
    auto it = lock.exact.find(std::string(name));
    assert(it != lock.exact.end());
    assert(satisfies_ge(range_min, it->second));
    return it->second;
}

// 无锁: 「解析」可能随时间漂移
SemVer resolve_floating(std::string_view /*name*/, SemVer /*range*/, SemVer catalog_latest) {
    return catalog_latest;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== semver_and_lockfiles ===\n";

    // --- 入门 ---
    auto a = parse_semver("1.84.0");
    auto b = parse_semver("1.85.1");
    auto c = parse_semver("2.0.0");
    assert(a.major == 1 && a.minor == 84);
    assert(a < b && b < c);
    assert(satisfies_caret(a, b));
    assert(!satisfies_caret(a, c));  // major 变了
    std::cout << "  1.84 -> 1.85 OK under caret; 2.0 not\n";

    // --- 进阶: lock ---
    Lockfile lock;
    lock.exact["fmt"] = parse_semver("10.2.1");
    lock.exact["spdlog"] = parse_semver("1.13.0");
    auto fmt_v = resolve_with_lock(lock, "fmt", parse_semver("10.0.0"));
    assert(fmt_v == parse_semver("10.2.1"));

    // 无锁: 今天 10.2.1 明天 catalog 变 11.0.0
    auto day1 = resolve_floating("fmt", parse_semver("10.0.0"), parse_semver("10.2.1"));
    auto day2 = resolve_floating("fmt", parse_semver("10.0.0"), parse_semver("11.0.0"));
    assert(day1 != day2);
    std::cout << "  without lock, resolve drifts: " << day1.major << "." << day1.minor << " vs " << day2.major << "."
              << day2.minor << '\n';

    // --- 专家: 工具映射 ---
    // vcpkg: builtin-baseline + (可选) overrides
    // conan: conan lock create / conan.lock
    // npm 类比: package-lock.json — 同样思想
    const char* vcpkg_lock = "builtin-baseline";
    const char* conan_lock = "conan.lock";
    assert(std::string_view(vcpkg_lock).find("baseline") != std::string_view::npos);
    assert(std::string_view(conan_lock).ends_with(".lock"));

    // patch 升级应兼容; major 需人工升级
    assert(satisfies_ge(parse_semver("1.2.3"), parse_semver("1.2.4")));
    assert(!satisfies_caret(parse_semver("1.2.3"), parse_semver("2.0.0")));

    std::cout << "semver_and_lockfiles: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section02/semver_and_lockfiles", run>;

}  // namespace
