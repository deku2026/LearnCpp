// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B9 clang-tidy)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : clang_tidy_lifetime_rules
// Topic id : part6/b/section06/clang_tidy_lifetime_rules
//
// 要点: 静态规则拦截悬垂句柄/use-after-move 等；展示「干净」写法。
// 参考: bugprone-dangling-handle, bugprone-use-after-move, cppcoreguidelines-*

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

// 易被 bugprone-dangling-handle 盯上的模式：返回指向局部的 view
// std::string_view bad() { std::string s="x"; return s; }

std::string good_return() {
    return std::string{"ok"};
}

std::string_view view_of(const std::string& s) {
    return s;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B9 clang-tidy lifetime-related checks ===\n";
    std::cout << "  key checks:\n";
    std::cout << "    bugprone-dangling-handle\n";
    std::cout << "    bugprone-use-after-move\n";
    std::cout << "    cppcoreguidelines-owning-memory / dangling\n";

    std::string s = good_return();
    std::string_view sv = view_of(s);
    assert(sv == "ok");

    // use-after-move：移动后只赋值或重置状态，不读值依赖
    std::string a = "data";
    std::string b = std::move(a);
    assert(b == "data");
    a = "reused";  // 明确重新赋值
    assert(a == "reused");

    std::vector<int> v{1, 2, 3};
    auto w = std::move(v);
    assert(w.size() == 3);
    v.clear();  // 明确处于有效但未指定内容的状态后使用
    v.push_back(9);
    assert(v.size() == 1);

    std::cout << "  run in CI: clang-tidy -checks=bugprone-*,cppcoreguidelines-*\n";
    std::cout << "clang_tidy_lifetime_rules: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section06/clang_tidy_lifetime_rules", run>;

}  // namespace
