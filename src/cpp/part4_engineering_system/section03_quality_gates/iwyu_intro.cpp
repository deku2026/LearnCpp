// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.3.3 IWYU include-what-you-use
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : iwyu_intro
// Topic id : part4/section03/iwyu_intro
//
// 要点: 每个文件只 include 真正用到的头; 去掉冗余 → 编译更快、依赖清晰。
//       模块化前尤其有用。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 符号 → 提供该符号的「规范头」
std::string_view canonical_header_for(std::string_view symbol) {
    if (symbol == "std::vector") {
        return "<vector>";
    }
    if (symbol == "std::string") {
        return "<string>";
    }
    if (symbol == "std::cout") {
        return "<iostream>";
    }
    if (symbol == "std::unique_ptr") {
        return "<memory>";
    }
    if (symbol == "assert") {
        return "<cassert>";
    }
    return "";
}

struct FileAnalysis {
    std::set<std::string> includes;
    std::set<std::string> symbols_used;
};

struct IwyuAdvice {
    std::vector<std::string> add;
    std::vector<std::string> remove;
};

IwyuAdvice advise(const FileAnalysis& f) {
    std::set<std::string> needed;
    for (const auto& sym : f.symbols_used) {
        auto h = canonical_header_for(sym);
        if (!h.empty()) {
            needed.emplace(h);
        }
    }
    IwyuAdvice a;
    for (const auto& n : needed) {
        if (!f.includes.contains(n)) {
            a.add.push_back(n);
        }
    }
    for (const auto& inc : f.includes) {
        if (!needed.contains(inc)) {
            a.remove.push_back(inc);
        }
    }
    return a;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== iwyu_intro ===\n";

    // --- 入门: 冗余 include ---
    FileAnalysis messy{
        {"<vector>", "<string>", "<map>", "<iostream>"},
        {"std::vector", "std::cout"},  // 实际只用 vector + cout
    };
    auto a = advise(messy);
    assert(a.remove.size() >= 1);
    // <map> <string> 应建议删除
    bool removes_map = false;
    for (const auto& r : a.remove) {
        if (r == "<map>" || r == "<string>") {
            removes_map = true;
        }
    }
    assert(removes_map);
    std::cout << "  remove suggestions:";
    for (const auto& r : a.remove) {
        std::cout << ' ' << r;
    }
    std::cout << '\n';

    // --- 进阶: 缺头(靠传递包含碰巧能编) ---
    FileAnalysis transitive{
        {"<iostream>"},  // 某实现可能传递带上 string, 但不可依赖
        {"std::string", "std::cout"},
    };
    auto b = advise(transitive);
    assert(!b.add.empty());
    bool add_string = false;
    for (const auto& x : b.add) {
        if (x == "<string>") {
            add_string = true;
        }
    }
    assert(add_string);
    std::cout << "  add <string> even if transitive include works today\n";

    // --- 专家: 本文件的「理想」include 集 ---
    FileAnalysis self{
        {"<cassert>", "<iostream>", "<set>", "<string>", "<string_view>", "<vector>"},
        {"assert", "std::cout", "std::string", "std::vector"},
    };
    auto c = advise(self);
    // set/string_view 可能被标 remove(我们简化表没映射) — 仅演示逻辑
    std::cout << "  self add=" << c.add.size() << " remove=" << c.remove.size() << '\n';

    assert(canonical_header_for("std::unique_ptr") == "<memory>");
    std::cout << "iwyu_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/iwyu_intro", run>;

}  // namespace
