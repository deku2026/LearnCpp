// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.3 FetchContent
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : fetch_content_declare
// Topic id : part4/section01/fetch_content_declare
//
// 要点: CMake FetchContent_Declare + MakeAvailable; 必须锁 GIT_TAG;
//       与 vcpkg/Conan/CPM 分工。本文件为可运行“声明模型”探测(不下载网络)。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace {

struct FetchDecl {
    std::string name;
    std::string git_repository;
    std::string git_tag;  // ⚠️ 锁定版本
    bool populated = false;
    bool available = false;
};

bool is_floating_ref(std::string_view tag) {
    return tag.empty() || tag == "master" || tag == "main" || tag == "develop" || tag == "HEAD";
}

bool is_reproducible(const FetchDecl& d) {
    return !d.name.empty() && !d.git_repository.empty() && !is_floating_ref(d.git_tag);
}

// 极简模拟 FetchContent 注册表(教学, 不访问网络)
struct FetchRegistry {
    std::unordered_map<std::string, FetchDecl> decls;

    void declare(FetchDecl d) { decls[d.name] = std::move(d); }

    bool make_available(std::string_view name) {
        auto it = decls.find(std::string(name));
        if (it == decls.end()) {
            return false;
        }
        if (!is_reproducible(it->second)) {
            return false;  // 拒绝未锁定依赖
        }
        it->second.populated = true;
        it->second.available = true;
        return true;
    }

    std::optional<FetchDecl> get(std::string_view name) const {
        auto it = decls.find(std::string(name));
        if (it == decls.end()) {
            return std::nullopt;
        }
        return it->second;
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== fetch_content_declare ===\n";

    // --- 入门: 文档示例 fmt@10.2.1 ---
    FetchDecl fmt{
        "fmt",
        "https://github.com/fmtlib/fmt.git",
        "10.2.1",
    };
    assert(is_reproducible(fmt));
    assert(!is_floating_ref(fmt.git_tag));

    FetchRegistry reg;
    reg.declare(fmt);
    assert(reg.make_available("fmt"));
    auto got = reg.get("fmt");
    assert(got && got->available && got->git_tag == "10.2.1");
    std::cout << "  " << got->name << " @ " << got->git_tag << " available\n";

    // --- 进阶: 浮动 tag 不可复现 → 拒绝 ---
    reg.declare(FetchDecl{"bad", "https://github.com/fmtlib/fmt.git", "master"});
    assert(!is_reproducible(*reg.get("bad")));
    assert(!reg.make_available("bad"));
    std::cout << "  reject GIT_TAG master/main (non-reproducible)\n";

    // 未知包
    assert(!reg.make_available("missing"));

    // --- 专家: 与包管理分工 ---
    // FetchContent: 源码拉入构建树, 适合少量依赖 / 与 CMake 一体
    // vcpkg/Conan: 二进制缓存、triplet、manifest 锁 — 团队规模更大
    // CPM: FetchContent 薄封装(见 section02/cpm_lightweight)
    struct Policy {
        std::string tool;
        bool needs_git_tag_lock;
        bool binary_cache;
    };
    std::vector<Policy> policies{
        {"FetchContent", true, false},
        {"CPM", true, false},
        {"vcpkg", true, true},
        {"Conan", true, true},
    };
    for (const auto& p : policies) {
        (void)p;
        assert(p.needs_git_tag_lock);
    }
    assert(!policies[0].binary_cache && policies[2].binary_cache);

    // target_link_libraries(myapp PRIVATE fmt::fmt) 心智: 名称在 MakeAvailable 后可用
    const std::string cmake_link = "fmt::fmt";
    assert(cmake_link.find("::") != std::string::npos);
    std::cout << "  after MakeAvailable: target_link_libraries(... " << cmake_link << ")\n";
    std::cout << "  repo note: LearnCpp itself vendors topics; deps via optional package mgrs\n";

    std::cout << "fetch_content_declare: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/fetch_content_declare", run>;

}  // namespace
