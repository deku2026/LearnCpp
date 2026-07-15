// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.6.1–4.6.2 GitHub Actions 矩阵
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : github_actions_matrix
// Topic id : part4/section06/github_actions_matrix
//
// 要点: os × compiler × build_type 笛卡尔积 + exclude; fail-fast: false;
//       钉版本 ubuntu-24.04 而非 -latest。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct Cell {
    std::string os;
    std::string compiler;
    std::string build_type;
};

bool excluded(const Cell& c) {
    // 文档 exclude 规则
    if (c.os == "windows-2022" && c.compiler == "gcc") {
        return true;
    }
    if (c.os == "macos-14" && c.compiler == "msvc") {
        return true;
    }
    if (c.os == "ubuntu-24.04" && c.compiler == "msvc") {
        return true;
    }
    if (c.os == "windows-2022" && c.compiler == "gcc") {
        return true;
    }
    // 合理补充: mac 上 gcc 少用可 exclude, 此处不排除 clang/gcc
    if (c.os.find("windows") != std::string::npos && c.compiler == "gcc") {
        return true;
    }
    return false;
}

std::vector<Cell> expand_matrix(const std::vector<std::string>& oses, const std::vector<std::string>& compilers,
                                const std::vector<std::string>& types) {
    std::vector<Cell> out;
    for (const auto& os : oses) {
        for (const auto& comp : compilers) {
            for (const auto& bt : types) {
                Cell c{os, comp, bt};
                if (!excluded(c)) {
                    out.push_back(std::move(c));
                }
            }
        }
    }
    return out;
}

// 钉版本: 拒绝 -latest
bool is_pinned_runner(std::string_view os) {
    return os.find("-latest") == std::string_view::npos;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== github_actions_matrix ===\n";

    const std::vector<std::string> oses{"ubuntu-24.04", "windows-2022", "macos-14"};
    const std::vector<std::string> compilers{"gcc", "clang", "msvc"};
    const std::vector<std::string> types{"Debug", "Release"};

    for (const auto& o : oses) {
        assert(is_pinned_runner(o));
    }
    assert(!is_pinned_runner("ubuntu-latest"));

    auto cells = expand_matrix(oses, compilers, types);
    // 全量 3*3*2=18, 排除: win+gcc(2), mac+msvc(2), ubuntu+msvc(2) = 6 → 12
    assert(cells.size() == 12);
    std::cout << "  matrix jobs after exclude=" << cells.size() << '\n';

    // fail-fast: false → 一个失败仍跑完, 看全貌
    const bool fail_fast = false;
    assert(!fail_fast);

    // 对照本仓库 presets: windows-ci / linux-ci / macos-ci
    std::cout << "  LearnCpp presets mirror matrix axes (os×config)\n";
    for (const auto& c : cells) {
        if (c.os == "ubuntu-24.04" && c.compiler == "clang" && c.build_type == "Debug") {
            std::cout << "  sample job: " << c.os << " " << c.compiler << " " << c.build_type << '\n';
        }
    }

    std::cout << "github_actions_matrix: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/github_actions_matrix", run>;

}  // namespace
