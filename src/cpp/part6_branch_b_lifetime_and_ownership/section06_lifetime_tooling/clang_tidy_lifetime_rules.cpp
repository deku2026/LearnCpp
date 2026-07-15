// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B9 clang-tidy)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : clang_tidy_lifetime_rules
// Topic id : part6/b/section06/clang_tidy_lifetime_rules
//
// 要点: 静态分析抓悬垂 handle / use-after-move；展示通过规矩写法。
// 参考: bugprone-dangling-handle, bugprone-use-after-move, cppcoreguidelines-*

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

// 易被 bugprone-dangling-handle 命中：
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
    std::cout << "    bugprone-infinite-loop (sometimes with bad lifetime)\n";
    std::cout << "    cppcoreguidelines-owning-memory / pro-bounds-*\n";

    std::string s = good_return();
    [[maybe_unused]] std::string_view sv = view_of(s);
    assert(sv == "ok");

    // use-after-move：移动后只赋新值或处于有效未指定状态，勿读旧值
    std::string a = "data";
    std::string b = std::move(a);
    assert(b == "data");
    a = "reused";
    assert(a == "reused");

    std::vector<int> v{1, 2, 3};
    auto w = std::move(v);
    assert(w.size() == 3);
    v.clear();
    v.push_back(9);
    assert(v.size() == 1);

    std::cout << "  run in CI:\n";
    std::cout << "    clang-tidy -checks=bugprone-*,cppcoreguidelines-* file.cpp\n";
    std::cout << "  B12: open tidy/Core Check and catch one dangling pattern\n";
    std::cout << "clang_tidy_lifetime_rules: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section06/clang_tidy_lifetime_rules", run>;

}  // namespace
