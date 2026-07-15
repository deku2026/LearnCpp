// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.2.3 CPM.cmake
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : cpm_lightweight
// Topic id : part4/section02/cpm_lightweight
//
// 要点: CPM 是 FetchContent 薄封装; gh:org/repo#tag 一行加依赖;
//       无需单独包管理器, 适合小项目/header-only。务必锁 tag。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct PackageRef {
    std::string host;  // gh
    std::string org;
    std::string repo;
    std::string tag;  // 必须钉死
};

// 解析 CPM 简写: "gh:fmtlib/fmt#10.2.1"
std::optional<PackageRef> parse_cpm_shorthand(std::string_view s) {
    // gh:org/repo#tag
    if (!s.starts_with("gh:")) {
        return std::nullopt;
    }
    s.remove_prefix(3);
    const auto slash = s.find('/');
    const auto hash = s.find('#');
    if (slash == std::string_view::npos || hash == std::string_view::npos || hash < slash) {
        return std::nullopt;
    }
    PackageRef r;
    r.host = "gh";
    r.org = std::string(s.substr(0, slash));
    r.repo = std::string(s.substr(slash + 1, hash - slash - 1));
    r.tag = std::string(s.substr(hash + 1));
    if (r.org.empty() || r.repo.empty() || r.tag.empty()) {
        return std::nullopt;
    }
    // 拒绝浮动分支名作为「锁」
    if (r.tag == "master" || r.tag == "main" || r.tag == "develop") {
        // 仍解析成功, 但 is_pinned 为 false
    }
    return r;
}

bool is_pinned(const PackageRef& r) {
    return r.tag != "master" && r.tag != "main" && r.tag != "develop" && !r.tag.empty();
}

std::string git_url(const PackageRef& r) {
    return "https://github.com/" + r.org + "/" + r.repo + ".git";
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== cpm_lightweight ===\n";

    // --- 入门: 解析文档示例 ---
    auto fmt = parse_cpm_shorthand("gh:fmtlib/fmt#10.2.1");
    assert(fmt.has_value());
    assert(fmt->org == "fmtlib" && fmt->repo == "fmt");
    assert(fmt->tag == "10.2.1");
    assert(is_pinned(*fmt));
    assert(git_url(*fmt) == "https://github.com/fmtlib/fmt.git");
    std::cout << "  " << git_url(*fmt) << " @ " << fmt->tag << '\n';

    // --- 进阶: 浮动 tag 危险 ---
    auto floating = parse_cpm_shorthand("gh:fmtlib/fmt#master");
    assert(floating && !is_pinned(*floating));
    std::cout << "  refuse floating branch for reproducibility\n";

    // 非法
    assert(!parse_cpm_shorthand("fmtlib/fmt").has_value());
    assert(!parse_cpm_shorthand("gh:onlyrepo").has_value());

    // --- 专家: 与 FetchContent 对照 ---
    // FetchContent_Declare(fmt GIT_REPOSITORY ... GIT_TAG 10.2.1)
    // CPMAddPackage("gh:fmtlib/fmt#10.2.1") 等价快捷方式
    struct FetchLike {
        std::string name;
        std::string git_repository;
        std::string git_tag;
    };
    FetchLike decl{fmt->repo, git_url(*fmt), fmt->tag};
    assert(decl.git_tag == "10.2.1");

    // 多包
    std::vector<std::string> lines{
        "gh:fmtlib/fmt#10.2.1",
        "gh:gabime/spdlog#v1.13.0",
        "gh:nlohmann/json#v3.11.3",
    };
    int pinned = 0;
    for (const auto& line : lines) {
        auto p = parse_cpm_shorthand(line);
        assert(p && is_pinned(*p));
        ++pinned;
    }
    assert(pinned == 3);
    std::cout << "  pinned packages=" << pinned << '\n';

    // 适用: header-only / 小项目; 大团队更常 vcpkg/Conan 二进制缓存
    std::cout << "cpm_lightweight: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section02/cpm_lightweight", run>;

}  // namespace
